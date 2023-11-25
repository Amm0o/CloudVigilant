// Main.cpp

#include "./libs/ProcessLister.h"
#include "./libs/HttpService/HttpService.h"
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <thread>

// Alias for the JSON object type
using json = nlohmann::json;

int main()
{
    ProcessLister ProcessLister;
    HttpService httpService;

    while (true)
    {
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
        }

        // Coverting JSON data to string
        std::string jsonString = jsonData.dump();
        // std::cout << jsonString << std::endl;

        // Send JSON string to the API and get the response
        httpService.sendData(jsonString, "https://localhost/api/dev/v1/processInfo");

        // Sleep for 5 seconds
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
