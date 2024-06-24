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
#include <iterator> 


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
                process.memUsage = getProcessMemUsage(pid);
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

// Get Process RAM usage
// This function will return RSS (Resident Set Size) in mbs from /proc/[pid]/statm
std::string ProcessLister::getProcessMemUsage(const std::string &pid)
{
    std::string statmPath = procDirectory + "/" + pid + "/statm";
    std::ifstream statmFile(statmPath);
    std::string line;
    long rss = 0;

    if (!statmFile.is_open()) {
        std::cerr << "Error: Could not open file " << statmPath << std::endl;
        return "-1";
    }

    try {
        if (std::getline(statmFile, line)) {
            std::istringstream iss(line);
            std::string value;
            if (iss >> value >> value) {
                rss = std::stol(value) * sysconf(_SC_PAGESIZE) / 1024; // Convert pages to KB and then to MB
            } else {
                std::cerr << "Error: Failed to parse RSS value from statm for PID " << pid << std::endl;
                return "-1";
            }
        }

        if (statmFile.bad()) {
            std::cerr << "Error: I/O error while reading file " << statmPath << std::endl;
            return "-1";
        }

        statmFile.close();


        rss /= 1024; // Convert KB to MB

        // std::cout << "RSS: " << rss << " for PID: "<< pid << std::endl;
        return std::to_string(rss);

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << " while processing PID " << pid << std::endl;
        return "-1";
    } catch (...) {
        std::cerr << "Unknown error occurred while processing PID " << pid << std::endl;
        return "-1";
    }
}
