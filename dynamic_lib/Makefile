
# Compiler
CXX = g++

# Python interpreter
PYTHON = python3

# Get compiler and linker flags from python3-config
PYTHON_CFLAGS = $(shell $(PYTHON)-config --includes)
PYTHON_LDFLAGS = $(shell $(PYTHON)-config --ldflags)

# Get pybind11 include flags
PYBIND_INCLUDES = $(shell $(PYTHON) -m pybind11 --includes)

# Get MySQL include flags and plugin directory
MYSQL_CFLAGS = $(shell mysql_config --include)
MYSQL_PLUGIN_DIR = $(shell mysql_config --plugindir)

# Files to copy
COPY_FILES = sentiment_analysis_function.py sentiment_model

# Compiler flags
CXXFLAGS = -std=c++11 -O2 -fPIC -shared $(PYTHON_CFLAGS) $(PYBIND_INCLUDES) $(MYSQL_CFLAGS)

# OS-specific adjustments for MacOS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # macOS-specific flags
    CXXFLAGS += -undefined dynamic_lookup -F/Library/Frameworks -framework Python
endif

# Source and target
SOURCES = sentiment.cpp outliers.cpp
TARGETS = $(addprefix $(MYSQL_PLUGIN_DIR)/, $(SOURCES:.cpp=.so))

# Build target
$(MYSQL_PLUGIN_DIR)/%.so: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(PYTHON_LDFLAGS)

save_model:
	python3 save_model.py

copy:
	cp -f -r $(COPY_FILES) $(MYSQL_PLUGIN_DIR)

all: $(TARGETS) save_model copy

# Clean target
clean:
	rm -f -r $(TARGETS) $(addprefix $(MYSQL_PLUGIN_DIR)/, $(COPY_FILES)) sentiment_model