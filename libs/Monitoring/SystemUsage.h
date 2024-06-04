#ifndef SYSTEM_USAGE_H
#define SYSTEM_USAGE_H

#include <vector>
#include <string>


struct MemoryInfo {

    long totalMemory;
    long usedMemory;
    double usedMemoryPercentege;

};

class SystemUsage {

public:
    double getCPUUsage();
    MemoryInfo getMemoryUsage();
private:
    std::vector<long> getCPUTimes();
    void parseLine(const std::string& line, long& value);

};


#endif

