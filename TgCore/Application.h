#pragma once

#include <tgbot/tgbot.h>

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
            typename Func,
            enableif<std::is_invocable_v<Func, TgBot::Message::Ptr>> = nullptr
        >
        void AddCommand(std::string command, Func&& callback)
        {
            this->getEvents().onCommand(std::move(command), 
                                        std::forward<Func>(callback));
        }

    private:
        void StartEventLoop();
    };
}