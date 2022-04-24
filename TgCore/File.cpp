#include "File.h"

namespace tg
{
    File::File(fs::path path, bool remove/* = true*/)
        : m_path(fs::canonical(std::move(path))),
          m_remove(remove)
    {
        std::ofstream stream{ m_path };

        if (!stream.is_open())
        {
            throw std::runtime_error{ "Can't create file: " + m_path.string() };
        }
    }

    File& File::operator<<(std::string_view data)
    {
        if (std::ofstream stream{ m_path })
        {
            stream << data;
        }
        else 
        {
            throw std::runtime_error{ "Can't open file: " + m_path.string() };
        }

        return *this;
    }

    File& File::operator>>(std::string &data)
    {
        if (std::ifstream stream{ m_path })
        {
            stream >> data;
        }
        else 
        {
            throw std::runtime_error{ "Can't open file: " + m_path.string() };
        }

        return *this;
    }
} // namespace tg
