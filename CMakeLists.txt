cmake_minimum_required(VERSION 3.10) # Specify the minimum version of CMake
project(cloud_vigilante) # Name your project

set(CMAKE_CXX_STANDARD 20) # Set C++20 as the standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories for header files
include_directories(${PROJECT_SOURCE_DIR}/libs)

# Specify the executable and its source files
add_executable(${PROJECT_NAME} Main.cpp libs/ProcessLister.cpp libs/HttpService/HttpService.cpp libs/DeviceInfo.cpp)

# If you have external libraries to link (e.g., nlohmann_json, libcurl)
include(FetchContent)


FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json curl)

