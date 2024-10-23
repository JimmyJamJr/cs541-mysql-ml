# Makefile for compiling main.cpp using g++

# Compiler
CXX = c++

# Python interpreter
PYTHON = python3

# Get compiler and linker flags from python3-config
PYTHON_CFLAGS = $(shell $(PYTHON)-config --includes)
PYTHON_LDFLAGS = $(shell $(PYTHON)-config --ldflags)
PYBIND_INCLUDE = /opt/homebrew/Cellar/pybind11/2.13.6_1/include

# Get pybind11 include path
# PYBIND11_INCLUDES = $(shell $(PYTHON) -m pybind11 --includes)

# Compiler flags
CXXFLAGS = -std=c++11 -O2 -undefined dynamic_lookup -I$(PYBIND_INCLUDE) $(PYTHON_INCLUDE) $(PYTHON_CFLAGS) -F/Library/Frameworks -framework Python

# Source and target
SOURCES = pybind_test.cpp
TARGET = pybind_test

# Build target
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(PYTHON_LDFLAGS)

# Clean target
clean:
	rm -f $(TARGET)