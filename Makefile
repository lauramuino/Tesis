# Define the compiler and flags
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -g

# Define the name of the final executable
TARGET   = tesis

# Define all source files (.cpp) and automatically generate object files (.o)
SRCS     = Graph.cpp Map.cpp TabuSearch.cpp main.cpp
OBJS     = $(SRCS:.cpp=.o)

.PHONY: all clean

# The 'all' target is the default and links the object files
all: $(TARGET)

# Rule to link all object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Generic rule to compile any .cpp file into a .o file
# $^ is all prerequisites, $@ is the target name
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)
