#include "Application.h"

namespace tg
{
    Application::Application(std::string token)
        : parent_type(token),
          m_token(std::move(token)),
          m_keepWork(true)
    {
        auto &events    = this->getEvents();
        const auto &api = this->getApi();

        this->AddCommand("start", [&api, this](TgBot::Message::Ptr messagePtr) 
        {
            api.sendMessage(messagePtr->chat->id,
                            "Welcome to the TgDictionary!");

            this->PrintLanguageChoose(messagePtr);
        });

        this->AddCommand("target", [this](TgBot::Message::Ptr messagePtr)
        {
            this->PrintLanguageChoose(messagePtr);
        });

        events.onNonCommandMessage([&api, this](TgBot::Message::Ptr messagePtr)
        {
            const auto id = messagePtr->chat->id;

            if (const auto &text = messagePtr->text; !text.empty())
            {
                this->m_translator.SetWords({ text });
                if (auto translatedOpt = this->m_translator.Translate())
                {
                    api.sendMessage(id, *translatedOpt);
                } 
                else 
                {
                    api.sendMessage(id, "Can't translate your text for unknown reason");
                }
            }

            if (const auto &doc = messagePtr->document)
            {
                // https://api.telegram.org/bot<bot_token>/getFile?file_id=the_file_id
                //  https://api.telegram.org/file/bot<token>/<file_path>
                // Режим перевода файлов
                static const std::string botLink  {"https://api.telegram.org/bot" + 
                                                   m_token + "/"};
                static const std::string fileLink {"https://api.telegram.org/file/bot" + 
                                                   m_token + "/"};

                const auto &fileId = doc->fileId;
                const auto &mime   = doc->mimeType;

                if (auto response = curl_request(botLink + "getFile?file_id=" + fileId,
                                                      {}, ""))
                {
                    auto json = nlohmann::json::parse(std::move(*response));

                    auto filePath = json.at("result")
                                         .at("file_path")
                                         .get<std::string>();

                    std::cout << filePath << std::endl;
                    if (auto fileContentsOpt = curl_request(fileLink + filePath, {}, ""))
                    {
                        this->m_translator.SetWords({ std::move(*fileContentsOpt) });
                        if (auto translatedOpt = this->m_translator.Translate())
                        {
                            std::string outFileName { fileId + ".txt" };

                            File out { outFileName };
                            out << *translatedOpt;
                            api.sendDocument(id, TgBot::InputFile::fromFile(outFileName, mime));
                        } 
                        else 
                        {
                            api.sendMessage(id, "Can't translate your text for unknown reason");
                        }
                    }
                }
            }
        });

        events.onCallbackQuery([&api, this](TgBot::CallbackQuery::Ptr callback)
        {
            std::cout << callback->data << std::endl;

            if (callback->data == "targetRu") 
            {
                api.sendMessage(callback->message->chat->id, 
                                "Target language set to Russian");
                this->m_translator.SetTargetLang("ru");   
            }
            else if (callback->data == "targetEn") 
            {
                api.sendMessage(callback->message->chat->id, 
                                "Target language set to English");
                this->m_translator.SetTargetLang("en");
            }
        });
    }


    ExitCode Application::Run()
    {
        try 
        {
            std::cout << "Bot username: " 
                      << this->getApi().getMe()->username 
                      << std::endl;

            this->StartEventLoop();

            return ExitCode::SUCCESS;
        }
        catch(TgBot::TgException &e)
        {
            std::cout << "LongPoll exception: " 
                      << e.what() 
                      << std::endl;

            return ExitCode::LONGPOLL_FAILURE;
        }
        catch (std::exception &e)
        {
            std::cout << "Unknown failure: " << e.what() << std::endl;
            // no return
        }

        return ExitCode::UNKNOWN_FAILURE;
    }

    void Application::PrintLanguageChoose(TgBot::Message::Ptr messagePtr)
    {
        static TgBot::InlineKeyboardMarkup::Ptr languageBtns = InitButtonsMarkup({
            { {"ru", "targetRu"}, {"en", "targetEn"} }
        });

        const auto &api = this->getApi();
        api.sendMessage(messagePtr->chat->id, 
                        "Choose target language.\nShow menu again - /target", 
                        false, 0, languageBtns);
    }

    void Application::StartEventLoop()
    {
        TgBot::TgLongPoll longPoll(*this);
        while (this->m_keepWork) {
            std::cout << "Started LongPoll" << std::endl;
            longPoll.start();
        }
    }

    TgBot::InlineKeyboardMarkup::Ptr Application::InitButtonsMarkup(buttons_container btns)
    {
        TgBot::InlineKeyboardMarkup::Ptr languageBtns{ new TgBot::InlineKeyboardMarkup };

        if (!languageBtns)
        {
            return languageBtns;
        }

        for (const auto &row : btns)
        {
            std::vector<TgBot::InlineKeyboardButton::Ptr> rowBtns{};

            for (const auto &[text, data] : row)
            {
                TgBot::InlineKeyboardButton::Ptr btn { new TgBot::InlineKeyboardButton };
                btn->text = text;
                btn->callbackData = data;
                rowBtns.emplace_back(std::move(btn));
            }

            languageBtns->inlineKeyboard.emplace_back(std::move(rowBtns));
        }

        return languageBtns;
    }
}