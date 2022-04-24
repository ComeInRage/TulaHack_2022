#include "Translator.h"

namespace tg
{
    void Translator::SetTargetLang(std::string targetLang)
    {
        m_targetLang = std::move(targetLang);
    }

    void Translator::SetWords(std::vector<std::string> words)
    {
        m_words = std::move(words);
    }

    std::optional<std::string> Translator::Translate()
    {
        nlohmann::json sourceMsgJson = *this;

        if (auto responseOpt = curl_post_request(m_url, 
                                                 m_headers, 
                                                 nlohmann::to_string(sourceMsgJson)))
        {
            auto outMsgJson = nlohmann::json::parse(std::move(*responseOpt));
            return outMsgJson["translations"].at(0)
                             ["text"]
                             .get<std::string>();
        }

        return std::nullopt;
    }
}