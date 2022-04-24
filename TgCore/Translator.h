#pragma once

#include "json.hpp"

#include <iostream>
#include <curl/curl.h>

namespace tg
{
    class Translator
    {
    private:
        friend nlohmann::adl_serializer<Translator>;
        
        std::string m_url{"https://translate.api.cloud.yandex.net/translate/v2/translate"};

        const std::vector<std::string> m_headers
        {
            "Content-Type: application/json",
            "Authorization: Bearer t1.9euelZqKkZXHlMeVkc2Lys2djJKVye3rnpWayseMxsuWzpiSzZaWnJObzY7l8_ciBm5s-e9xJmFD_t3z92I0a2z573EmYUP-.L42GmXXNXVlK2HZtFC-dbNepBCl6tssoIDskVHBhjOYZB4KZTwcIta_Qc-faj5c46fvhEM4UvZcnOVmBq7U_Bw"
        };

        const std::string m_requestType {"POST"};

        std::vector<std::string> m_words;
        std::string m_targetLang;
        std::string m_folderId{"b1ga217tv36i08f8halq"};

    public:
        Translator(std::vector<std::string> words = {}, std::string targetLang = "en") noexcept
            : m_words(std::move(words)),
              m_targetLang(std::move(targetLang))
        {}

        void SetTargetLang(std::string);
        void SetWords(std::vector<std::string> words);
        std::string Translate();

    };

    size_t ResponseReader(char *contents, size_t size, size_t nmemb, std::string *userp);
} // namespace tg

namespace nlohmann
{
    template<>
    struct adl_serializer<tg::Translator> {

        static void to_json(nlohmann::json &j, const tg::Translator& value) {
            j["folderId"] = value.m_folderId;
            j["texts"] = value.m_words;
            j["targetLanguageCode"] = value.m_targetLang;
        }

        static void from_json(const nlohmann::json &j, tg::Translator& value) {
            // same thing, but with the "from_json" method
        }
    };
}