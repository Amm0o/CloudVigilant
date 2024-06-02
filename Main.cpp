// Main.cpp

#include "./libs/ProcessLister.h"
#include "./libs/HttpService/HttpService.h"
#include "./libs/DeviceInfo.h"
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
    DeviceFetcher fetcher;

    // Get the device information
    DeviceInfo deviceInfo = fetcher.getDeviceInfo();

    while (true)
    {
        auto processInfo = ProcessLister.getProcessInfo();

        json jsonData = json::array();

        // Grab the machine info  
        json machineProperties = {
            {"DeviceID", deviceInfo.deviceID},
            {"TenantID", deviceInfo.tenantID},
            {"DeviceName", deviceInfo.deviceName},
            {"MacAddress", deviceInfo.macAddress},
            {"IpAddress", deviceInfo.ipAddress}
        };

        // Push the info to the final json
        jsonData.push_back(machineProperties);

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


        // Output Json for debugging
        // std::cout << jsonData << std::endl;

        // Send JSON string to the API and get the response
        // httpService.sendData(jsonString, "https://localhost/api/dev/v1/processInfo");


        // Sleep for 5 seconds
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
