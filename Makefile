CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Libraries to link
LIBS = -lmosquitto -lpthread

# Source files
SRCS = LoggerService.cpp Logger.cpp LoggerImpl.cpp Utils.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = LoggerService

# Default target to build the executable
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
