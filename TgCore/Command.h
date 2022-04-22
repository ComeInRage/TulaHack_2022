#pragma once

#include <tgbot/tgbot.h>

#include <string>
#include <string_view>
#include <type_traits>

namespace tg
{
  template<bool value>
  using enableif = std::enable_if_t<value, std::nullptr_t>;

  // Класс является сущностью команды для бота
  template
  <
    typename Func,
    enableif<std::is_invocable_v<Func, Command<Func>>> = nullptr
  >
  class Command
  {
  private:
    std::string m_command;
    Func m_callback;

  public:
    Command()                           = delete; // Non default-constructible
    Command(const Command&)             = default;
    Command(Command&&)                  = default;
    Command& operator=(const Command&)  = default;
    Command& operator=(Command&&)       = default;
    ~Command()                          = default;

    Command(TgBot::Bot &bot, std::string command, Func&& callback) noexcept 
      : m_command(std::move(command)), 
        m_callback(std::forward<Func>(callback))
    {
      
    }
  };
}
