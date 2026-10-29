#include "url_shortener/url_shortener.h"

UrlShortener::UrlShortener()
{
    registerRoutes();
}

void UrlShortener::registerRoutes()
{
    CROW_ROUTE(app, "/")([]()
    {
        return "URL Shortener server is running!";
    });

    CROW_ROUTE(app, "/health")([]()
    {
        return crow::response(200, "Server looks healthy!");
    });

    CROW_ROUTE(app, "/checkDB")([]()
    {
        const char* connectionString = std::getenv("DATABASE_URL");

        if(connectionString == nullptr){
            return crow::response(500, "Database String not available!");
        }

        Database database{connectionString};
        database.testConnection();

        return crow::response(200, "Database connected Successfully!");
    });
}

void UrlShortener::run()
{
    app.port(8080)
       .multithreaded()
       .run();
}