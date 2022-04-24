#include "Application.h"

namespace tg
{
    Application::Application(std::string token)
        : parent_type(std::move(token)),
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
                api.sendMessage(id, this->m_translator.Translate());
            }
            if (const auto &doc = messagePtr->document)
            {
                // Режим перевода файлов
                const auto &filename = doc->fileName;
                const auto &fileId   = doc->fileId;

                // File file {fileId + "_" + filename};
                // continue
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