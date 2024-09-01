#ifndef SYSTEM_USAGE_H
#define SYSTEM_USAGE_H

#include <vector>
#include <string>

struct MemoryInfo
{

    long totalMemory;
    long usedMemory;
    double usedMemoryPercentege;
};

struct DiskInfo
{
    long long total;
    long long used;
    long long free;
};

class SystemUsage
{

public:
    double getCPUUsage();
    MemoryInfo getMemoryUsage();
    DiskInfo getDiskUsage();

private:
    std::vector<long> getCPUTimes();
    void parseLine(const std::string &line, long &value);
};

#endif
