#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <iostream>

namespace py = pybind11;

int main(int argc, char *argv[]) {
    // Run Python interpreter
    py::scoped_interpreter guard{};

    // Get the Python math module
    py::module_ math = py::module_::import("math");
    // Get the Python sqrt function from the math module
    py::object sqrt_func = math.attr("sqrt");
    // Call math.sqrt(16)
    py::object result = sqrt_func(16);

    // Convert the result to a C++ type
    double cpp_result = result.cast<double>();

    std::cout << "The square root of 16 is: " << cpp_result << std::endl;
        
    return 0;
}