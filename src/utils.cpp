#include "utils.h"
#include "config.h"
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <sstream>
#include <iomanip>

namespace UtilityNamespace {

    std::string authenticate() {
        std::string url = "https://test.deribit.com/api/v2/public/auth";
        std::string payload = "{\"jsonrpc\":\"2.0\", \"method\":\"public/auth\", \"params\":{\"grant_type\":\"client_credentials\", \"client_id\":\"" + API_KEY + "\", \"client_secret\":\"" + SECRET_KEY + "\"}, \"id\":1}";

        // POST request to authenticate
        std::string response = UtilityNamespace::sendPostRequest(url, payload);

        // parsing the response to get the access token
        auto json_response = nlohmann::json::parse(response);
        if (json_response.contains("result") && json_response["result"].contains("access_token")) {
            std::string access_token = json_response["result"]["access_token"];
            //std::cout << "Access Token: " << access_token << std::endl;
            return access_token;
        }
        else {
            std::cerr << "Authentication failed. Response: " << response << std::endl;
            throw std::runtime_error("Authentication failed.");
        }
    }

    // callback func to write data received from cURL to a string
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
        size_t newLength = size * nmemb;
        s->append((char*)contents, newLength);
        return newLength;
    }


    // Helper function to URL-encode a string
    std::string urlEncode(const std::string& value) {
        std::ostringstream encoded;
        for (auto ch : value) {
            if (isalnum(ch)) {
                encoded << ch;
            }
            else {
                encoded << '%' << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << int((unsigned char)ch);
            }
        }
        return encoded.str();
    }

    // Function to concatenate query parameters as a URL-encoded string
    std::string buildQueryString(const std::unordered_map<std::string, std::string>& params) {
        std::ostringstream queryString;
        bool first = true;
        for (const auto& [key, value] : params) {
            if (!first) queryString << "&";
            queryString << urlEncode(key) << "=" << urlEncode(value);
            first = false;
        }
        return queryString.str();
    }
    // Function to perform HTTP GET request with Authorization header and query parameters
    std::string sendGetRequestWithAuth(const std::string& url, const std::unordered_map<std::string, std::string>& queryParams, const std::string& access_token) {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        // Build the full URL with query parameters
        std::string fullUrl = url + "?" + buildQueryString(queryParams);

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            // Set the full URL with query parameters
            curl_easy_setopt(curl, CURLOPT_URL, fullUrl.c_str());

            // Set up the authorization header
            struct curl_slist* headers = nullptr;
            std::string authHeader = "Authorization: Bearer " + access_token;
            headers = curl_slist_append(headers, authHeader.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            // Callback to capture the response
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            // Perform the GET request
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
            }

            // Cleanup
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();
        return readBuffer;
    }

    std::string UtilityNamespace::sendPostRequestWithAuth(const std::string& url, const std::string& payload, const std::string& authHeader) {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl) {
            struct curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, authHeader.c_str());

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
            }

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
        return readBuffer;
    }

    // function to perform HTTP POST requests
    std::string UtilityNamespace::sendPostRequest(const std::string& url, const std::string& payload) {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl) {
            struct curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            std::string authHeader = "Authorization: Bearer " + API_KEY;
            headers = curl_slist_append(headers, authHeader.c_str());

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
        return readBuffer;
    }
}