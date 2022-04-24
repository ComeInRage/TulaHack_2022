#include "Translator.h"

namespace tg
{
    size_t ResponseReader(char *contents, size_t size, size_t nmemb, std::string *userp)
    {
        if (userp)
        {
            userp->reserve(size * nmemb);
            userp->append(contents, size * nmemb);
        }
        else
        {
            throw std::invalid_argument{"User pointer not defined"};
        }
        return size * nmemb;
    }

    void Translator::SetTargetLang(std::string targetLang)
    {
        m_targetLang = std::move(targetLang);
    }

    void Translator::SetWords(std::vector<std::string> words)
    {
        m_words = std::move(words);
    }

    std::string Translator::Translate()
    {
        nlohmann::json sourceMsgJson = *this;

        CURL *curl;
        CURLcode res;
        std::string readBuffer{};

        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());
            
            curl_slist* headers = nullptr;
            for (const auto &header : m_headers) 
            {
                headers = curl_slist_append(headers, header.c_str());
            }

            auto body = nlohmann::to_string(sourceMsgJson);

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseReader);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            
            //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }

        auto outMsgJson = nlohmann::json::parse(std::move(readBuffer));
        readBuffer = outMsgJson["translations"].at(0)
                               ["text"]
                               .get<std::string>();

        return readBuffer; // copy elision
    }
}