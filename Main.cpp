// Main.cpp

#include "./libs/ProcessLister.h"
#include "./libs/HttpService/HttpService.h"
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

// Alias for the JSON object type
using json = nlohmann::json;

int main()
{
    ProcessLister ProcessLister;
    auto processInfo = ProcessLister.getProcessInfo();

    json jsonData = json::array();

    for (const auto &process : processInfo)
    {
        // Create JSON object for each process
        json processJson = {
            {"ProcessPID", process.pid},
            {"ProcessName", process.name},
            {"ProcessCommand", process.command},
            {"ProcessCpuUsage", process.cpuUsage}};

        // Add JSON object to JSON array
        jsonData.push_back(processJson);

        // Print process info to console for debugging
        // std::cout << "Process ID: " << process.pid << ", "
        //           << "Name: " << process.name << ", "
        //           << "Command: " << process.command << ", "
        //           << "CPU Usage: " << process.cpuUsage << "%" << std::endl;
    }

    // Coverting JSON data to string
    std::string jsonString = jsonData.dump();

    // Send JSON string to the API
    HttpService httpService;
    httpService.sendData(jsonString, "http://localhost:8000");

    return 0;
}
