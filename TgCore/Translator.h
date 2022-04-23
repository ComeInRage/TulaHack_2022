#pragma once

#include "json.hpp"

#include <iostream>
#include "curl2/include/curl/curl.h"

namespace tg
{
    class Translator
    {
    private:
        using headers_container = std::vector<std::pair<std::string, std::string>>;

        friend nlohmann::adl_serializer<Translator>;
        
        std::string m_url{"https://translate.api.cloud.yandex.net/translate/v2/translate"};

        const headers_container m_headers {
            {"Content-Type", "application/json"},
            {"Authorization", "Bearer t1.9euelZqZipyJjIyTlJWczp6RkcnIye3rnpWayseMxsuWzpiSzZaWnJObzY7l8_d_QG9s-e99ZyxC_t3z9z9vbGz5731nLEL-.Y1PzNu3h3nxaYYZ_PZrL0pf8AHlbusGCUo6ZUH4JIJ-63S5J2NAS3ERu__W1Lww5fSjxWL17AFeQDuDwknTZCA"}
        };

        const std::string m_requestType {"POST"};

        std::vector<std::string> m_words;
        std::string m_targetLang;
        std::string m_folderId{"b1ga217tv36i08f8halq"};

    public:
        Translator(std::vector<std::string> words, std::string targetLang = "en") noexcept
            : m_words(std::move(words)),
              m_targetLang(std::move(targetLang))
        {}

        size_t write(void *contents, size_t size, size_t nmemb, void *userp);
        std::string Translate();

    };
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