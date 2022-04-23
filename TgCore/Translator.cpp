#include "Translator.h"
#include <iostream>
#include <curl/curl.h>

namespace tg
{
    size_t Translator::write(void *contents, size_t size, size_t nmemb, void *userp)
    {
        if (userp)
        {
            ((std::string*)userp)->reserve(size);
            ((std::string*)userp)->append((char*)contents, size);
        }
        return size;
    }

    std::string Translator::Translate()
    {
        nlohmann::json json = *this;

        CURL *curl;
        CURLcode res;
        std::string readBuffer{};

        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());
            
            curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, "Authorization: Bearer t1.9euelZqKkZXHlMeVkc2Lys2djJKVye3rnpWayseMxsuWzpiSzZaWnJObzY7l8_ciBm5s-e9xJmFD_t3z92I0a2z573EmYUP-.L42GmXXNXVlK2HZtFC-dbNepBCl6tssoIDskVHBhjOYZB4KZTwcIta_Qc-faj5c46fvhEM4UvZcnOVmBq7U_Bw");

            size_t (Translator::*func)(void *, size_t, size_t, void *);
            func = &Translator::write;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, 
            [&readBuffer](char *contents, size_t size, size_t nmemb, std::string *userp)
            {
                readBuffer.reserve(size);
                readBuffer.append((char*)contents, size);
                return size;
            });
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"folderId\": \"b1ga217tv36i08f8halq\",\"texts\": [\"Hello\", \"World\"],\"targetLanguageCode\": \"ru\"}");
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            std::cout << (res == CURLE_OK) << " " << readBuffer << std::endl;
        }

        return readBuffer;
    }
}