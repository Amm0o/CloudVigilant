// ProcessLister.h

#ifndef PROCESSLISTER_H
#define PROCESSLISTER_H

#include <string>
#include <vector>
#include <map>

// ProcessLister class
class ProcessLister
{
public:
    ProcessLister();
    std::map<std::string, std::tuple<std::string, std::string, std::string>> getProcessInfo();
    std::string getProcessName(const std::string &pid);
    std::string getProcessCommand(const std::string &pid);
    std::string getProcessCpuUsage(const std::string &pid);

private:
    std::string procDirectory;
};

#endif // PROCESSLISTER_H