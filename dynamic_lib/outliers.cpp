/*
https://dev.mysql.com/doc/extending-mysql/8.0/en/adding-loadable-function.html
*/

#if defined(MYSQL_SERVER)
#include <m_string.h> /* To get stpcpy() */
#else
/* when compiled as standalone */
#include <string.h>
#endif

#include <ctype.h>
#include <mysql.h>
#include <string>

typedef struct {
    double *values;
    size_t capacity;
    size_t size;
    char *result;
    unsigned long result_length;
} OutlierData;

int compare_doubles(const void *a, const void *b) {
    double arg1 = *(const double *)a;
    double arg2 = *(const double *)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

double percentile(double *sorted_values, size_t size, double percent) {
    double position = (size + 1) * percent / 100.0;
    size_t index = (size_t)position;
    double fraction = position - index;
    if (index == 0) {
        return sorted_values[0];
    } else if (index >= size) {
        return sorted_values[size - 1];
    } else {
        return sorted_values[index - 1] + fraction * (sorted_values[index] - sorted_values[index - 1]);
    }
}

char* compute_outliers(double *values, size_t size, unsigned long *result_length) {
    // Sort the values
    qsort(values, size, sizeof(double), compare_doubles);

    // Compute Q1, Q3, and IQR
    double q1 = percentile(values, size, 25);
    double q3 = percentile(values, size, 75);
    double iqr = q3 - q1;

    double lower_bound = q1 - 1.5 * iqr;
    double upper_bound = q3 + 1.5 * iqr;

    // Collect outliers
    char *result = NULL;
    size_t result_capacity = 1024;
    size_t result_length_local = 0;

    result = (char *)malloc(result_capacity);
    if (result == NULL) {
        // Handle memory allocation failure
        *result_length = 0;
        return NULL;
    }

    result[0] = '\0';

    for (size_t i = 0; i < size; i++) {
        if (values[i] < lower_bound || values[i] > upper_bound) {
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "%.10g", values[i]);
            size_t buffer_length = strlen(buffer);

            // Add comma if not the first outlier
            if (result_length_local > 0) {
                if (result_length_local + 1 >= result_capacity) {
                    result_capacity *= 2;
                    result = (char *)realloc(result, result_capacity);
                    if (result == NULL) {
                        *result_length = 0;
                        return NULL;
                    }
                }
                strcat(result, ",");
                result_length_local += 1;
            }

            // Ensure there's enough space
            if (result_length_local + buffer_length >= result_capacity) {
                result_capacity = result_length_local + buffer_length + 1024;
                result = (char *)realloc(result, result_capacity);
                if (result == NULL) {
                    *result_length = 0;
                    return NULL;
                }
            }

            // Append the outlier value
            strcat(result, buffer);
            result_length_local += buffer_length;
        }
    }

    *result_length = result_length_local;

    // If no outliers were found, return an empty string or NULL
    if (result_length_local == 0) {
        free(result);
        *result_length = 0;
        return NULL;
    }

    return result;
}


// C_MODE_START;
extern "C"
{
    bool outliers_init(UDF_INIT * initid, UDF_ARGS *args, char *message);
    void outliers_deinit(UDF_INIT * initid);
    void outliers_clear(UDF_INIT * initid, char * is_null, char * error);
    void outliers_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
    void outliers_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
    char * outliers(UDF_INIT * initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error);
}
// C_MODE_END;

// Call xxx_init() to let the aggregate function allocate any memory it needs for storing results.
// The initialization function for xxx().
bool outliers_init(UDF_INIT * initid, UDF_ARGS *args, char * message) {
    if (args->arg_count != 1) {
        strcpy(message, "wrong arguments: outlier() requires a column name as argument");
        return 1;
    }

    if (args->arg_type[0] != REAL_RESULT && args->arg_type[0] != INT_RESULT) {
        strcpy(message, "outlier() requires a numeric argument");
        return 1;
    }

    initid->const_item = 0;     // This UDF is not deterministic
    initid->decimals = 0;
    initid->maybe_null = 1;     // This UDF can return NULL
    initid->max_length = 0;     // The result can be of any length

    // Allocate memory for the result
    OutlierData * data = (OutlierData *) malloc(sizeof(OutlierData));
    if (data == NULL) {
        strcpy(message, "Could not allocate memory");
        return 1;
    }

    // Allocate memory for outlier data structure
    data->capacity = 1024;
    data->size = 0;
    data->values = (double *) malloc(data->capacity * sizeof(double));
    if (data->values == NULL) {
        strcpy(message, "Could not allocate memory");
        return 1;
    }
    data->result = NULL;
    data->result_length = 0;
    initid->ptr = (char *) data;
    return 0;
}

void outliers_deinit(UDF_INIT * initid) {
    // Free the allocated memory
    OutlierData * data = (OutlierData *) initid->ptr;
    if (data != NULL) {
        if (data->values != NULL) {
            free(data->values);
        }
        if (data->result != NULL) {
            free(data->result);
        }
        free(data);
    }
}

void outliers_clear(UDF_INIT * initid, char * is_null, char * error) {
    OutlierData * data = (OutlierData *) initid->ptr;
    data->size = 0;

    // Free previous result if any
    if (data->result != NULL) {
        free(data->result);
        data->result = NULL;
        data->result_length = 0;
    }
}

void outliers_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error) {
    OutlierData * data = (OutlierData *) initid->ptr;

    // Get the value from the arguments
    double value = 0.0;
    if (args->arg_type[0] == REAL_RESULT) {
        value = *((double *) args->args[0]);
    }
    else if (args->arg_type[0] == INT_RESULT) {
        value = (double) (*((long long *) args->args[0]));
    }
    else {
        *error = 1;
        return;
    }

    // Check if we need to expand our array
    if (data->size >= data->capacity) {
        data->capacity *= 2;
        double *new_values = (double *) realloc(data->values, data->capacity * sizeof(double));
        if (new_values == NULL) {
            *error = 1;
            return;
        }
        data->values = new_values;
    }

    // Add the value to the array
    data->values[data->size] = value;
    data->size++;
}

char * outliers(UDF_INIT * initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) {
    OutlierData *data = (OutlierData *)initid->ptr;

    if (data->size == 0) {
        *is_null = 1;
        return NULL;
    }

    data->result = compute_outliers(data->values, data->size, &data->result_length);
    if (data->result == NULL) {
        *is_null = 1;
        return NULL;
    }

    *length = data->result_length;
    return data->result;
}