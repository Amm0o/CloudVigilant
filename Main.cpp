// Main.cpp

#include "./libs/ProcessLister.h"
#include <iostream>

int main()
{
    ProcessLister ProcessLister;
    auto processInfo = ProcessLister.getProcessInfo();

    for (const auto &[pid, info] : processInfo)
    {
        std::cout << "Process ID: " << pid << ", Name: " << info.first << ", Command: " << info.second << std::endl;
    }

    return 0;
}