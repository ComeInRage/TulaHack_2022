#pragma once

#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

namespace tg
{
    class File
    {
    private:
        fs::path m_path;
        bool m_remove;
    
    public:
        File() noexcept
            : m_path{},
              m_remove(true)
        {}

        File(const File&)            = delete;
        File(File&&)                 = default;
        File& operator=(const File&) = delete;
        File& operator=(File&&)      = default;
        
        ~File() noexcept
        {
            if (m_remove)
            {
                std::error_code err {};
                fs::remove(m_path, err);
            }
        }

        File(fs::path path, bool remove = true);

        File& operator<<(std::string_view data);
        File& operator>>(std::string &data);

        bool Exists()           const noexcept;
        std::string Filename()  const;
        std::string Extension() const;
        const fs::path& Path()  const noexcept;

    };
}