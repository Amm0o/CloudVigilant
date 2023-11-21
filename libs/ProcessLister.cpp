// ProcessLister.cpp

// Path: libs/ProcessLister.cpp
#include "ProcessLister.h"
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

ProcessLister::ProcessLister()
{
    procDirectory = "/proc";
}

std::map<std::string, std::pair<std::string, std::string>> ProcessLister::getProcessInfo()
{
    std::map<std::string, std::pair<std::string, std::string>> processInfo;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(procDirectory.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            std::string pid = ent->d_name;
            if (std::all_of(pid.begin(), pid.end(), isdigit))
            {
                std::string name = getProcessName(pid);
                std::string command = getProcessCommand(pid);
                processInfo[pid] = std::make_pair(name, command);
            }
        }
        // Release handle on directory
        closedir(dir);
    }
    else
    {
        perror("Could not open /proc directory");
    }

    return processInfo;
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