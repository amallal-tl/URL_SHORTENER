#pragma once

#include <string>

class Database{
    public:
        explicit Database(std::string connectionString);

        Database();

        void testConnection() const;
    
    private:
        std::string connectionString_;
};