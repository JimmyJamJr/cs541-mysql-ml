/*
https://dev.mysql.com/doc/extending-mysql/8.0/en/adding-loadable-function.html
*/
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

#if defined(MYSQL_SERVER)
#include <m_string.h> /* To get stpcpy() */
#else
/* when compiled as standalone */
#include <string.h>
#endif

#include <ctype.h>
#include <mysql.h>

#include <cstring>
#include <string>
#include <thread>
#include <mutex>

namespace py = pybind11;
py::object sent;

std::once_flag init_flag;


// C_MODE_START;
extern "C"
{
    bool sentiment_init(UDF_INIT * initid, UDF_ARGS *args, char *message);
    void sentiment_deinit(UDF_INIT * initid);
    char * sentiment(UDF_INIT * initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error);
}
// C_MODE_END;

// Call xxx_init() to let the aggregate function allocate any memory it needs for storing results.
// The initialization function for xxx().
bool sentiment_init(UDF_INIT * initid, UDF_ARGS *args, char * message) {
    if (args->arg_count != 1 || args->arg_type[0] != STRING_RESULT)
    {
        strcpy(message, "wrong arguments: sentiment() requires a column name as arguments");
        return 1;
    }

    initid->const_item = 1;     // This UDF is deterministic
    initid->maybe_null = 1;     // This UDF can return NULL
    initid->max_length = 0;     // The result can be of any length

    try {
        std::call_once(init_flag, []() {
            py::initialize_interpreter();

            // Change cwd to the directory where the python script is
            py::module_ os = py::module_::import("os");
            py::object chdir = os.attr("chdir");
            chdir("../lib/plugin");
//            chdir("/Users/jimmy/Documents/Python/cs541-mysql-ml/dynamic_lib/");

            sent = py::module_::import("sentiment_analysis_function");
        });
    }
    catch (const std::exception &e) {
        strncpy(message, e.what(), MYSQL_ERRMSG_SIZE);
        message[MYSQL_ERRMSG_SIZE - 1] = '\0';
        return 1; // Failure
    }

//    py::module_ os = py::module_::import("os");
//    py::object getcwd = os.attr("getcwd");
//    py::object cwd_result = getcwd();
//    std::string cwd_result_cpp = cwd_result.cast<std::string>();

//    py::gil_scoped_acquire acquire;
//
//    auto sent = py::module_::import("sentiment_analysis_function");
//    auto sentiment_analysis_func = sent.attr("sentiment_analysis");
////
//    py::object test_func = sent.attr("test");
//    py::object sentiment_result = test_func();
//    std::string sentiment_result_cpp = sentiment_result.cast<std::string>();
//    strcpy(message, sentiment_result_cpp.c_str());

//    strcpy(message, cwd_result_cpp.c_str());
//    return 1;
//    strcpy(message, "test2");
    // Allocate memory for the result
    initid->ptr = (char *)malloc(16384); // Adjust size as needed
    if (!initid->ptr) {
        strncpy(message, "Memory allocation failed", MYSQL_ERRMSG_SIZE);
        message[MYSQL_ERRMSG_SIZE - 1] = '\0';
        return 1;
    }
//    initid->max_length = 16384;

    return 0;
}

void sentiment_deinit(UDF_INIT * initid) {
    // Free the allocated memory
    if (initid->ptr) {
        free(initid->ptr);
        initid->ptr = nullptr;
    }
}

char * sentiment(UDF_INIT * initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) {
    try {
        // Get the input string
        std::string input_value(args->args[0], args->lengths[0]);

        // Get the python sentiment analysis function
        py::gil_scoped_acquire acquire; // Acquire the GIL
        py::object sent = py::module_::import("sentiment_analysis_function");
        py::object sent_func = sent.attr("sentiment_analysis");

        // Call the sentiment analysis function on the input
        py::object sentiment_result = sent_func(input_value);

        // Convert the sentiment result into cstring
        std::string sentiment_result_cpp = sentiment_result.cast<std::string>();
        char *res_ptr = (char *)initid->ptr;
        strcpy(res_ptr, sentiment_result_cpp.c_str());
        *length = sentiment_result_cpp.length();
//        *length = input_value.length();
        return res_ptr;
    }
    catch (const py::error_already_set& e) {
        strcpy(error, e.what());
        *is_null = 1;
        return nullptr;
    }

    //     Test with sentiment test function
//    try {
//        py::gil_scoped_acquire acquire; // Acquire the GIL
////        auto sent = py::module_::import("sentiment_analysis_function");
//        py::object test_func = sent.attr("test");
//        py::object sentiment_result = test_func();
//        std::string sentiment_result_cpp = sentiment_result.cast<std::string>();
//
//        char *res_ptr = (char *)initid->ptr;
//        strcpy(res_ptr, sentiment_result_cpp.c_str());
//        *length = sentiment_result_cpp.length();
//        return res_ptr;
//    } catch (const py::error_already_set& e) {
//        strcpy(error, e.what());
//        *is_null = 1;
//        return nullptr;
//    }

// Test with hello world
//    try {
//        py::gil_scoped_acquire acquire;
//
//        py::object py_result = py::eval("\"Hello from Python\"");
//        std::string result_str = py::str(py_result).cast<std::string>();
//
//        // Use initid->ptr for the result as before...
//        char *res_ptr = (char *)initid->ptr;
//        strcpy(res_ptr, result_str.c_str());
//        *length = result_str.length();
//        return res_ptr;
//
//    }
//    catch (const py::error_already_set& e) {
//        strncpy(error, e.what(), MYSQL_ERRMSG_SIZE);
//        error[MYSQL_ERRMSG_SIZE - 1] = '\0';
//        *is_null = 1;
//        return nullptr;
}