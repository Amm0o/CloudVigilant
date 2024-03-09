
// DeviceInfo.cpp : implementation file

#include "DeviceInfo.h"
#include <cstdio>
#include <nlohmann/json.hpp>
#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream>
#include <array>
#include <memory>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <vector>


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
        throw std::runtime_error("Unable to open file: /opt/cloud-vigilante/cloudVigilanteOnboarding.json Device not onboarded!");
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
    std::vector<std::string> ipv4Cache;
    std::vector<std::string> ipv6Cache;
    struct ifaddrs *ifaddr;

    // Handle not being able to get the ips
    if (getifaddrs(&ifaddr) == -1) {
        throw std::runtime_error("Unbale to getifaddrs");
        return "Error";
    }

    for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {

        // Skipping loopback and cases where there's no address
        if(ifa->ifa_addr == NULL || ifa->ifa_flags & IFF_LOOPBACK) {
            continue;
        }

        int family = ifa->ifa_addr->sa_family;
        // Ipv4 Scenario
        if(family == AF_INET) {
            char host[NI_MAXHOST];

            getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in ),
                        host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

            ipv4Cache.push_back(host);

            // printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, host);

        } else if(family == AF_INET6) { // Ipv6 Scenario
            char host[NI_MAXHOST];

            getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in6),
                        host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

            ipv6Cache.push_back(host);

            // printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, host);

        }

    }


    freeifaddrs(ifaddr);

    if(ipv4Cache.empty()) {
        std::cerr << ("No Ipv4 was encountered") << std::endl;
        return "NO IP";
    } else {
        return ipv4Cache[0];
    }


    return ipv4Cache[0];
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
