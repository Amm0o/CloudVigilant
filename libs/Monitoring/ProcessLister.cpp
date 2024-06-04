// ProcessLister.cpp

// Path: libs/ProcessLister.cpp
#include "ProcessLister.h"
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <memory>
#include <array>
#include <cstdio>

ProcessLister::ProcessLister()
{
    procDirectory = "/proc";
}

std::vector<ProcessInfo> ProcessLister::getProcessInfo()
{
    std::vector<ProcessInfo> processList;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(procDirectory.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            std::string pid = ent->d_name;
            if (std::all_of(pid.begin(), pid.end(), isdigit))
            {
                ProcessInfo process;
                process.pid = pid;
                process.name = getProcessName(pid);
                process.command = getProcessCommand(pid);
                process.cpuUsage = getProcessCpuUsage(pid);
                processList.push_back(process);
            }
        }
        // Release handle on directory
        closedir(dir);
    }
    else
    {
        perror("Could not open /proc directory");
    }

    return processList;
}

// Get Process Name
std::string ProcessLister::getProcessName(const std::string &pid)
{
    std::string line;

    // Open stream for status file
    std::ifstream statusFile(procDirectory + "/" + pid + "/status");

    if (statusFile.is_open())
    {
        while (getline(statusFile, line))
        {
            if (line.substr(0, 6) == "Name:\t")
            {
                // return line
                return line.substr(6);
            }
        }

        // Close the file stream
        statusFile.close();
    }

    // Return unknown if name not found
    return "Unkonwn";
}

// Get Process Command
std::string ProcessLister::getProcessCommand(const std::string &pid)
{
    // Get file stream for command line
    std::ifstream cmdlineFile(procDirectory + "/" + pid + "/cmdline");
    std::string command;

    if (cmdlineFile.is_open())
    {
        // Get command line for process
        getline(cmdlineFile, command);
        std::replace(command.begin(), command.end(), '\0', ' '); // Replace null characters with spaces
        // Close handle on the file
        cmdlineFile.close();
    }

    return command.empty() ? "Unknown" : command;
}

// Get Process CPU Usage
#include <iterator> // Add this line to include the <iterator> header file

std::string ProcessLister::getProcessCpuUsage(const std::string &pid)
{
    std::string command = "ps -p " + pid + " -o %cpu --no-headers";
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    // Remove trailing new lines or spaces
    result.erase(std::remove_if(result.begin(), result.end(), isspace), result.end());

    return result.empty() ? "Unknown" : result;
}
