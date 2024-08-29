#include <iostream>
#include <curl/curl.h>
#include <procon_api.h>

// Callback function to handle the response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string GetRequest(const std::string& url, const std::string& token) {
    CURL* curl;
    CURLcode res;
    std::string readData;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readData);

        // Set the Procon-Token header
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Procon-Token: " + token).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            throw std::runtime_error("curl_easy_perform() failed: " + std::string(curl_easy_strerror(res)));
        }  else if (readData == "AccessTimeError") {
            throw std::runtime_error("Access Time Error");
        }
    }

    return readData;
}

// Function to perform a POST request
std::string PostRequest(const std::string& url, const std::string& token, const std::string& jsonData) {
    CURL* curl;
    CURLcode res;
    std::string readData;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readData);

        // Set the headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("Procon-Token: " + token).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            throw std::runtime_error("curl_easy_perform() failed: " + std::string(curl_easy_strerror(res)));
        }
    }

    return readData;
}