#include "Application.h"

namespace tg
{
    Application::Application(std::string token)
        : parent_type(std::move(token)),
            m_keepWork(true)
    {
        this->AddCommand("start", [this](TgBot::Message::Ptr messagePtr)
        {
            std::cout << "Started!" << std::endl;
            this->m_keepWork = true;
        });

        this->AddCommand("stop", [this](TgBot::Message::Ptr messagePtr)
        {
            std::cout << "Stopped!" << std::endl;
            this->m_keepWork = false;
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

    void Application::StartEventLoop()
    {
        TgBot::TgLongPoll longPoll(*this);
        while (this->m_keepWork) {
            std::cout << "Started LongPoll" << std::endl;
            longPoll.start();
        }
    }
}