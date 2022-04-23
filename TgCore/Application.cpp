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
          
            this->SendInputChoose(messagePtr);
            this->SendOutputChoose(messagePtr);
        });


        events.onNonCommandMessage([&api](TgBot::Message::Ptr messagePtr)
        {
            if (auto doc = messagePtr->document)
            {
                api.sendMessage(messagePtr->chat->id, "document" + doc->fileName);
            }
        });

        events.onCallbackQuery([&api](TgBot::CallbackQuery::Ptr callback)
        {
            auto startsWith = [&callback](std::string_view text)
            {
                return StringTools::startsWith(callback->data, "inputRu");
            };

            if (startsWith("inputRu"))
            {
                api.sendMessage(callback->message->chat->id, 
                                "Input language set to Russian");
            }
            if (startsWith("inputEn")) 
            {
                
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
        catch (...)
        {
            std::cout << "Unknown failure" << std::endl;
            // no return
        }

        return ExitCode::UNKNOWN_FAILURE;
    }

    void Application::SendInputChoose(TgBot::Message::Ptr messagePtr)
    {
        static TgBot::InlineKeyboardMarkup::Ptr languageBtns = InitButtonsMarkup({
            { {"ru", "inputRu"}, {"en", "inputEn"} }
        });

        const auto &api = this->getApi();
        api.sendMessage(messagePtr->chat->id, "Choose input language:", false, 0, languageBtns);
    }

    void Application::SendOutputChoose(TgBot::Message::Ptr messagePtr)
    {
        static TgBot::InlineKeyboardMarkup::Ptr languageBtns = InitButtonsMarkup({
            { {"ru", "outputRu"}, {"en", "outputEn"} }
        });

        const auto &api = this->getApi();
        api.sendMessage(messagePtr->chat->id, "Choose output language:", false, 0, languageBtns);
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