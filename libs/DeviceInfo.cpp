
// DeviceInfo.cpp : implementation file

#include "DeviceInfo.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <array>
#include <memory>
#include <stdexcept>

std::string DeviceFetcher::execCommand(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    return result;
}

// Get the Device ID
nlohmann::json DeviceFetcher::getOnboardingInfo()
{

    std::ifstream file("/opt/cloud-vigilante/cloudVigilanteOnboarding.json");
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file /opt/cloud-vigilante/cloudVigilanteOnboarding.json");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    auto json = nlohmann::json::parse(buffer.str()); // Parse the buffer with the file data

    return json;
}

// Get the Device Name
std::string DeviceFetcher::getDeviceName()
{
    return execCommand("hostname");
}

// Get the MAC Address
std::string DeviceFetcher::getMacAddress()
{
    return execCommand("cat /sys/class/net/eth0/address");
}

// Get the IP Address
std::string DeviceFetcher::getIpAddress()
{
    return execCommand("hostname -I | cut -d' ' -f1");
}

DeviceInfo DeviceFetcher::getDeviceInfo()
{
    DeviceInfo info;
    auto onboardingInfo = getOnboardingInfo();

    info.tenantID = onboardingInfo["TenantID"];
    info.deviceID = onboardingInfo["DeviceID"];
    info.deviceName = getDeviceName();
    info.macAddress = getMacAddress();
    info.ipAddress = getIpAddress();

    return info;
}