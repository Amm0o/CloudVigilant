// HttpService.cpp

#include "HttpService.h"
#include <curl/curl.h>
#include <string>
#include <iostream>

// Callback function to handle the incoming data
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

void HttpService::sendData(const std::string &jsonData, const std::string &apiEndpoint)
{
    // Initialize cURL session
    CURL *curl = curl_easy_init();
    if (curl)
    {
        // Set the api endpoint
        curl_easy_setopt(curl, CURLOPT_URL, apiEndpoint.c_str());

        // Set to use SSL/TLS
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); // Enable for secure connections

        // Set HTTP method to POST
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "/home/angelo/.local/share/mkcert/rootCA.pem");

        // Set the JSON payload as the POST data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

        // Set the content-type header
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_slist_append(headers, "Custom: Test");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set up the callback function to handle the response
        std::string readBuffer;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else
        {
            // Print the response data
            std::cout << "Received data: " << readBuffer << std::endl;
        }

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}
