#include "HttpService.h"
// sudo apt-get install libcurl4-openssl-dev
//          ```
//      - On CentOS or Fedora-based systems, you can run the following command in the terminal:
//          ```
//          sudo dnf install libcurl-devel
//          ```

// 2. Update the include statement in your code:
//      - Replace `#include <curl/curl.h>` with `#include <curl.h>`.

// After making these changes, the #include errors and the "cannot open source file" error should be resolved.

// Here's the updated code:
#include <curl/curl.h>

void HttpService::sendData(const std::string &jsonData, const std::string &apiEndpoint)
{
    // Initialize cURL session
    CURL *curl = curl_easy_init();
    if (curl)
    {
        // Set the api endpoint
        curl_easy_setopt(curl, CURLOPT_URL, apiEndpoint.c_str());

        // Set HTTP method to POST
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Set the JSON payload as the POST data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

        // Set the content-type header
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}
