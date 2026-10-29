#pragma once

#include <optional>
#include <string>

class Database{
    public:
        explicit Database(std::string connectionString);

        Database();

        void testConnection() const;

        void saveUrl(
            const std::string& shortCode,
            const std::string& OriginalUrl
        );

        std::optional<std::string> findUrl(
            const std::string& shortCode
        ) const;
    
    private:
        std::string connectionString_;
};