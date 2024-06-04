#include "SystemUsage.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>



std::vector<long> SystemUsage::getCPUTimes() {
    
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);
    std::istringstream iss(line);
    std::string cpu;
    long time;
    std::vector<long> times;

    iss >> cpu; // Skip the 'cpu' prefix
    while (iss >> time) {
        times.push_back(time);
    }
    return times;

}


double SystemUsage::getCPUUsage() {

    std::vector<long> prevTimes = getCPUTimes();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::vector<long> currTimes = getCPUTimes();
    
    long prevIdle = prevTimes[3] + prevTimes[4];
    long currIdle = currTimes[3] + currTimes[4];

    long prevNonIdle = prevTimes[0] + prevTimes[1] + prevTimes[2] + prevTimes[5] + prevTimes[6] + prevTimes[7];
    long currNonIdle = currTimes[0] + currTimes[1] + currTimes[2] + currTimes[5] + currTimes[6] + currTimes[7];

    long prevTotal = prevIdle + prevNonIdle;
    long currTotal = currIdle + currNonIdle;

    double totald = currTotal - prevTotal;
    double idled = currIdle - prevIdle;

    return ((totald - idled) / totald) * 100;

}



void SystemUsage::parseLine(const std::string& line, long& value) {
    std::istringstream iss(line);
    std::string key;
    std::string unit;
    iss >> key >> value >> unit;
}


MemoryInfo SystemUsage::getMemoryUsage() {
    std::ifstream file("/proc/meminfo");
    std::string line;
    long totalMemory = 0;
    long freeMemory = 0;
    long bufferMemory = 0;
    long cachedMemory = 0;

    while (std::getline(file, line)) {
        if (line.find("MemTotal:") == 0) parseLine(line, totalMemory);
        else if (line.find("MemFree:") == 0) parseLine(line, freeMemory);
        else if (line.find("Buffers:") == 0) parseLine(line, bufferMemory);
        else if (line.find("Cached:") == 0) parseLine(line, cachedMemory);

        if (totalMemory > 0 && freeMemory > 0 && bufferMemory > 0 && cachedMemory > 0)
            break; // We have collected all necessary info, no need to parse further
    }


    long usedMemory = totalMemory - freeMemory - bufferMemory - cachedMemory;
    double usedMemoryPercentage = (double)usedMemory / (double)totalMemory * 100;


    // Make values mb
    totalMemory /= 1024;
    usedMemory /= 1024;

    return {totalMemory, usedMemory, usedMemoryPercentage};
} 
