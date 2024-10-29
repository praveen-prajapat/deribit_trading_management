#pragma once
#include <unordered_map>
#include <curl/curl.h>
#include <string>
#include <iostream>

namespace UtilityNamespace {

    std::string authenticate();
    std::string sendGetRequestWithAuth(const std::string& url, const std::unordered_map<std::string, std::string>& queryParams, const std::string& access_token);
    std::string sendPostRequest(const std::string& url, const std::string& postFields);
    std::string sendPostRequestWithAuth(const std::string& url, const std::string& postFields, const std::string& token);
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);
}