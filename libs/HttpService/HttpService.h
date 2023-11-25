// HttpService.h

#ifndef HTTPSERVICE_H
#define HTTPSERVICE_H

#include <string>

class HttpService
{
public:
    void sendData(const std::string &jsonData, const std::string &apiEndpoint);
};

#endif // HTTPSERVICE_H
