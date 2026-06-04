# Makefile for mync

# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11

# Name of the executable
TARGET = mync

# Source files
SOURCES = mync.cpp

# Build rule
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Clean rule
clean:
	rm -f $(TARGET)

.PHONY: all clean