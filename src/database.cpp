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

void Database::saveUrl(const std::string& shortCode, const std::string& originalUrl){
    // Open a PostgreSQL connection using the stored connection string.
    pqxx::connection connection{connectionString_};
    // Start a transaction so the insert can be committed safely.
    pqxx::work transaction{connection};

    // Insert the URL using parameters rather than building SQL with strings.
    transaction.exec(
        "INSERT INTO shortened_urls (short_code, original_url) "
        "VALUES ($1, $2)",
        pqxx::params{shortCode, originalUrl}
    );

    // Commit the transaction to persist the new row.
    transaction.commit();
}

std::optional<std::string> Database::findUrl(const std::string& shortCode) const {
    // Open a PostgreSQL connection using the stored configuration.
    pqxx::connection connection{connectionString_};

    // Use a read-only transaction because this function only reads data.
    pqxx::read_transaction transaction{connection};

    // Look up the original URL using a parameterized query.
    const auto result = transaction.exec(
        "SELECT original_url FROM shortened_urls WHERE short_code = $1",
        pqxx::params{shortCode}
    );

    if(result.empty()) 
        return std::nullopt;
    
    return result[0][0].as<std::string>();
}

std::vector<std::pair<std::string, std::string>> Database::getAllUrls() const
{
    pqxx::connection connection{connectionString_};
    pqxx::read_transaction transaction{connection};

    const auto result = transaction.exec(
        "SELECT short_code, original_url "
        "FROM shortened_urls "
        "ORDER BY created_at DESC"
    );

    std::vector<std::pair<std::string, std::string>> urls;

    for (const auto& row : result)
    {
        urls.emplace_back(
            row[0].as<std::string>(),
            row[1].as<std::string>()
        );
    }

    return urls;
}
