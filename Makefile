# Compiler settings - Can be customized.
CXX = g++
CXXFLAGS = -std=c++20 -Wall -g
LDFLAGS = -lcurl

# Define targets
TARGET = cloud_vigilante
SRCS = Main.cpp libs/Monitoring/ProcessLister.cpp libs/HttpService/HttpService.cpp libs/Monitoring/DeviceInfo.cpp libs/Monitoring/SystemUsage.cpp

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

# Production build
prod: CXXFLAGS += -DPROD
prod: clean $(TARGET)
