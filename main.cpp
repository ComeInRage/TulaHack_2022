#include <iostream>
#include <tgbot/tgbot.h>

int main() {
    // Здесь пример использования бота.
    // Нужно накатить синтаксического сахара и легковесных оберток вокруг этой шляпы
    // Либо забить и юзать так

    // TgBot::Bot bot("5332685416:AAE8ElGu8XG58WiXfTujjCyk-TtYCgtMl-c");
    // bot.getEvents().onCommand("start2", [&bot](TgBot::Message::Ptr message) {
    //     bot.getApi().sendMessage(message->chat->id, "Андрей ololo!");
    // });
    // bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
    //     std::cout << "User wrote: " << message->text << std::endl;
    //     if (StringTools::startsWith(message->text, "/start")) {
    //         return;
    //     }
    //     bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
    // });
    // try {
    //     std::cout << "Bot username: " << bot.getApi().getMe()->username << std::endl;
    //     TgBot::TgLongPoll longPoll(bot);
    //     while (true) {
    //         std::cout << "Started event poll" << std::endl;
    //         longPoll.start();
    //     }
    // } catch (TgBot::TgException& e) {
    //     std::cout << "error: " << e.what() << std::endl;
    // }
    return 0;
}