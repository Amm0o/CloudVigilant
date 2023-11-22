// Main.cpp

#include "./libs/ProcessLister.h"
#include <iostream>
#include <tuple>

int main()
{
    ProcessLister ProcessLister;
    auto processInfo = ProcessLister.getProcessInfo();

    for (const auto &[pid, info] : processInfo)
    {
        // Extract the process name and command from the tuple
        const auto &[name, command, cpuUsage] = info;

        std::cout << "CPU Usage: " << cpuUsage << "%, "
                  << "Process ID: " << pid << ", "
                  << "Name: " << name << ", "
                  << "Command: " << command << std::endl;
    }

    return 0;
}