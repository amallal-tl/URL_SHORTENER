#include "url_shortener/url_shortener.h"

#include <exception>

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

    // Crow calls this lambda when a client sends GET /checkDB.
    CROW_ROUTE(app, "/checkDB")([this]()
    {
        try{
            // Reuse the Database object owned by UrlShortener.
            database_.testConnection();
            return crow::response(
                200,
                "Database connected successfully!!!"
            );
        }catch(const std::exception& exception){
            // Log the error on the server, not in the HTTP response.
            std::cerr << "Database check failed: "
                      << exception.what() << '\n';

            return crow::response(
                500,
                "Database connection failed."
            );
        }
    });
}

void UrlShortener::run()
{
    app.port(8080)
       .multithreaded()
       .run();
}