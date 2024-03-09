# Compiler settings - Can be customized.
CXX = gcc
CXXFLAGS = -std=c++20 -Wall -g
LDFLAGS = -lcurl


# Define targets
TARGET = cloud_vigilante
SRCS = Main.cpp libs/ProcessLister.cpp libs/HttpService/HttpService.cpp libs/DeviceInfo.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)


# Compilation
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
