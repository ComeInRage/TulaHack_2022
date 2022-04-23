#pragma once

#include <tgbot/tgbot.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>
#include <type_traits>

namespace tg
{
    enum ExitCode : int
    {
        SUCCESS = 0,
        UNKNOWN_FAILURE = 1,
        LONGPOLL_FAILURE = 2,
    };

    template<bool value>
    using enableif = std::enable_if_t<value, std::nullptr_t>;

    class Application : public TgBot::Bot
    {
    private:
        using parent_type = TgBot::Bot;

        bool m_keepWork;

    public:
        using parent_type::Bot;
        using parent_type::operator=;

        Application(std::string token);

        ExitCode Run();
        
        template
        <
            typename Func
        >
        void AddCommand(std::string command, Func&& callback)
        {
            this->getEvents().onCommand(std::move(command), 
                                        std::forward<Func>(callback));
        }

    private:
        void SendInputChoose(TgBot::Message::Ptr messagePtr);
        void SendOutputChoose(TgBot::Message::Ptr messagePtr);
        void StartEventLoop();
    };

    TgBot::InlineKeyboardMarkup::Ptr InitButtonsMarkup(std::vector<std::unordered_map<std::string, std::string>> btns);
}