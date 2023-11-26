// DeviceInfo.h

#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <string>
#include <nlohmann/json.hpp>

struct DeviceInfo
{
    std::string deviceID;
    std::string tenantID;
    std::string deviceName;
    std::string macAddress;
    std::string ipAddress;
};

class DeviceFetcher
{
public:
    DeviceInfo getDeviceInfo();

private:
    std::string infoDirectory;
    nlohmann::json getOnboardingInfo();
    std::string getDeviceName();
    std::string getMacAddress();
    std::string getIpAddress();
    std::string execCommand(const char *cmd);
};

#endif // DEVICEINFO_H
