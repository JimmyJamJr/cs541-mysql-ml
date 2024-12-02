# Makefile for compiling main.cpp using g++

# Compiler
CXX = g++

# Python interpreter
PYTHON = python3

# Get compiler and linker flags from python3-config
PYTHON_CFLAGS = $(shell $(PYTHON)-config --includes)
PYTHON_LDFLAGS = $(shell $(PYTHON)-config --ldflags)
PYBIND_INCLUDE = /opt/homebrew/Cellar/pybind11/2.13.6_1/include
MYSQL_INCLUDE = /usr/local/mysql/include
COPY_FILES = sentiment_analysis_function.py sentiment_model
MYSQL_PLUGIN_DIR = /usr/local/mysql/lib/plugin/

# Compiler flags
CXXFLAGS = -std=c++11 -O2 -fPIC -shared -undefined dynamic_lookup -I$(PYBIND_INCLUDE) $(PYTHON_INCLUDE) -I$(MYSQL_INCLUDE) $(PYTHON_CFLAGS) -F/Library/Frameworks -framework Python

# Source and target
SOURCES = sentiment.cpp
TARGET = $(MYSQL_PLUGIN_DIR)sentiment.so

# Build target
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(PYTHON_LDFLAGS)

copy:
	cp -f -r $(COPY_FILES) $(MYSQL_PLUGIN_DIR)

all: $(TARGET) copy

# Clean target
clean:
	rm -f $(TARGET)