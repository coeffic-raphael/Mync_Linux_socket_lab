# Makefile for mync

# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11 -fprofile-arcs -ftest-coverage

# Name of the executable
TARGET = mync

# Source files
SOURCES = mync.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Build rule
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) -lgcov

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for generating coverage report
coverage: all
	./$(TARGET)
	gcov $(SOURCES)

# Clean rule
clean:
	rm -f $(TARGET) $(OBJECTS) *.gcno *.gcda *.gcov

.PHONY: all clean coverage