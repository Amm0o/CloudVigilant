// Main.cpp

#include "./libs/Monitoring/ProcessLister.h"
#include "./libs/HttpService/HttpService.h"
#include "./libs/Monitoring/DeviceInfo.h"
#include "./libs/Monitoring/SystemUsage.h"
#include <cfenv>
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

        json jsonData = json::array();

        // Get resource consumption per process
        auto processInfo = ProcessLister.getProcessInfo();

        // Get total resource consumption
        SystemUsage systemUsage;
        MemoryInfo memoryInfo = systemUsage.getMemoryUsage();

        json totalConsumption = {
            {"TotalCpu", systemUsage.getCPUUsage()},
            {"TotalMemory", memoryInfo.totalMemory},
            {"UsedMemory", memoryInfo.usedMemory},
            {"UsedMemoryP", memoryInfo.usedMemoryPercentege}

        };


        jsonData.push_back(totalConsumption);
        

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
        // httpService.sendData(jsonData, "https://localhost/api/dev/v1/processInfo");
        
        // Converto to string to send data
        std::string jsonString = jsonData.dump();
        httpService.sendData(jsonString, "http://localhost:8080/api/v1/postMetrics");


        // Sleep for 5 seconds
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
