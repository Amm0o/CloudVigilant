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

    while (true)
    {
        // Get the device information
        DeviceInfo deviceInfo = fetcher.getDeviceInfo();

        // Get resource consumption per process
        auto processInfo = ProcessLister.getProcessInfo();

        // Get total resource consumption
        SystemUsage systemUsage;
        MemoryInfo memoryInfo = systemUsage.getMemoryUsage();
        DiskInfo diskInfo = systemUsage.getDiskUsage();

        json totalConsumption = {
            {"TotalCpu", systemUsage.getCPUUsage()},
            {"TotalMemory", memoryInfo.totalMemory},
            {"UsedMemory", memoryInfo.usedMemory},
            {"UsedMemoryP", memoryInfo.usedMemoryPercentege},
            {"TotalDisk", diskInfo.total},
            {"UsedDisk", diskInfo.used},
            {"FreeDisk", diskInfo.free}};

        std::cout << "Disk Usage: " << diskInfo.used << std::endl;

        // Grab the machine info
        json machineProperties = {
            {"DeviceID", deviceInfo.deviceID},
            {"TenantID", deviceInfo.tenantID},
            {"DeviceName", deviceInfo.deviceName},
            {"MacAddress", deviceInfo.macAddress},
            {"IpAddress", deviceInfo.ipAddress},
            {"TimeStamp", deviceInfo.timeStamp}};

        std::cout << "TimeStamp: " << deviceInfo.timeStamp << std::endl;

        // Grab process performance metrics
        json processArray = json::array();
        for (const auto &process : processInfo)
        {
            // Create JSON object for each process
            json processJson = {
                {"ProcessPID", process.pid == "Unknown" ? -1 : std::stoi(process.pid)},
                {"ProcessName", process.name},
                {"ProcessCommand", process.command},
                {"ProcessCpuUsage", process.cpuUsage == "Unknown" ? -1 : std::stoi(process.cpuUsage)},
                {"ProcessMemUsage", process.memUsage == "Unknown" ? -1 : std::stoi(process.memUsage)}};

            // Add each process to the process array
            processArray.push_back(processJson);
        }

        // Build the final json
        json jsonData = {
            {"totalConsumption", totalConsumption},
            {"machineProperties", machineProperties},
            {"processInfo", processArray}};

        // Output Json for debugging
        // std::cout << jsonData << std::endl;

        // Converto to string to send data
        std::string jsonString = jsonData.dump();
        // std::cout << jsonString << std::endl;
#ifdef PROD
        httpService.sendData(jsonString, "http://cloudvigilante.anoliveira.com/api/v1/postMetrics");
#else
        httpService.sendData(jsonString, "http://localhost:8080/api/v1/postmetrics");
#endif

#ifdef PROD
        // Sleep for 5 seconds
        std::this_thread::sleep_for(std::chrono::seconds(30));
#else
        std::this_thread::sleep_for(std::chrono::seconds(5));
#endif
    }

    return 0;
}
