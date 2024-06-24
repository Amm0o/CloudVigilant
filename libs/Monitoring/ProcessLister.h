// ProcessLister.h

#ifndef PROCESSLISTER_H
#define PROCESSLISTER_H

#include <string>
#include <vector>
#include <map>

struct ProcessInfo
{
    std::string pid;
    std::string name;
    std::string command;
    std::string cpuUsage;
    std::string memUsage;
};

// ProcessLister class
class ProcessLister
{
public:
    ProcessLister();
    std::vector<ProcessInfo> getProcessInfo();
    std::string getProcessName(const std::string &pid);
    std::string getProcessCommand(const std::string &pid);
    std::string getProcessCpuUsage(const std::string &pid);
    std::string getProcessMemUsage(const std::string &pid);
private:
    std::string procDirectory;
};

#endif // PROCESSLISTER_H