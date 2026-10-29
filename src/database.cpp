#include "url_shortener/database.h"

#include <pqxx/pqxx>
#include <iostream>
#include <utility>


Database::Database(std::string connectionString): connectionString_(std::move(connectionString)){}

Database::Database(){
    const char* connnectionString = std::getenv("DATABASE_URL");

    if(connnectionString == nullptr){
        throw std::runtime_error(
            "DATABASE_URL environment variable is not set"
        );
    }

    connectionString_ = connnectionString;
}

void Database::testConnection() const {
    pqxx::connection connection{connectionString_};
    pqxx::read_transaction transaction{connection};

    const auto result = transaction.exec(
        "SELECT current_database(), current_user" 
    );

    std::cout << "Connected to database successfully!" << std::endl;
    std::cout << "Database : " << result[0][0].c_str() << std::endl;
    std::cout << "User : " << result[0][1].c_str() << std::endl;
}