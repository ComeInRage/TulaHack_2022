#include "curl_request.h"

namespace tg
{
    size_t ResponseReader(char *contents, size_t size, size_t nmemb, std::string *userp)
    {
        if (userp)
        {
            userp->reserve(size * nmemb);
            userp->append(contents, size * nmemb);
            return size * nmemb;
        }
        return 0;
    }

    std::optional<std::string> curl_request(const std::string &url,
                                                 const std::vector<std::string> &headers,
                                                 const std::string &body)
    {
        CURLcode returnCode;
        std::string responseBuffer{};

        if(CURL *curl = curl_easy_init()) {

            if (!headers.empty())
            {
                curl_slist* headers_list = nullptr;
                for (const auto &header : headers) 
                {
                    headers_list = curl_slist_append(headers_list, 
                                                    header.c_str());
                }
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers_list);
            }

            if (!body.empty())
            {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
            }

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseReader);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);
            
            #ifdef _DEBUG
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            #endif

            returnCode = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (returnCode == CURLE_OK)
            {
                return std::move(responseBuffer);
            }
        }

        return std::nullopt;
    }

    void curl_download_file(const std::string &link)
    {
        CURLcode returnCode;

        if(CURL *curl = curl_easy_init()) {

            curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
            
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            #ifdef _DEBUG
            #endif

            returnCode = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
    }
}