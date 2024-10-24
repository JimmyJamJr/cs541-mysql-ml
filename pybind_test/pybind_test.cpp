#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream

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

     // Print Pytorch version from python
    py::module_ torch = py::module_::import("torch");
    py::object torch_version = torch.attr("__version__");
    std::string torch_version_cpp = torch_version.cast<std::string>();
    std::cout << "PyTorch Version: " << torch_version_cpp << std::endl;


    // Read csv file
    std::vector<std::string> texts;
    std::ifstream fin("../Sentiment_Analysis/sentimentdataset.csv");
    std::vector<std::string> row;
    std::string line, word, temp;
    while (getline(fin, line)) {
        row.clear();
        std::stringstream s(line);
        while (std::getline(s, word, ',')) {
            row.push_back(word);
        }

        std::string text = row[2];
        texts.push_back(text);
    }
    fin.close();

    // Use the sentiment_analysis_function to run sentiment analysis on the test data
    py::module_ sent = py::module_::import("sentiment_analysis_function");
    py::object sentiment_analysis_func = sent.attr("sentiment_analysis");
    py::object sentiment_result = sentiment_analysis_func(texts);
    std::vector<bool> sentiment_result_cpp = sentiment_result.cast<std::vector<bool>>();

    for (bool b : sentiment_result_cpp) {
        std::cout << (b ? "Positive" : "Negative") << std::endl;
    }

    return 0;
}