#pragma once

#include <curl/curl.h>

#include <vector>
#include <optional>
#include <string>
#include <string_view>

namespace tg
{
    size_t ResponseReader(char *contents, size_t size, size_t nmemb, std::string *userp);

    std::optional<std::string> curl_request(const std::string &url,
                                                 const std::vector<std::string> &headers,
                                                 const std::string &body);

    void curl_download_file(const std::string &link);
}