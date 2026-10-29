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
                "Database connected successfully"
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
  
    CROW_ROUTE(app, "/urls")
        .methods(crow::HTTPMethod::POST)
    ([this](const crow::request& request)
    {
        const auto body = crow::json::load(request.body);

        if (!body)
        {
            return crow::response(400, "Invalid JSON body.");
        }

        if (!body.has("url"))
        {
            return crow::response(400, "Missing 'url' field.");
        }

        try
        {
            const std::string originalUrl = body["url"].s();
            const std::string code = urlService_.shorten(originalUrl);

            crow::json::wvalue result;
            result["short_code"] = code;
            result["short_url"] = "http://localhost:8080/r/" + code;

            crow::response response(201, result.dump());
            response.set_header("Content-Type", "application/json");
            return response;
        }
        catch (const std::exception& exception)
        {
            std::cerr << "URL creation failed: "
                    << exception.what() << '\n';

            return crow::response(500, "Failed to create short URL.");
        }
    });


    CROW_ROUTE(app, "/urls")
        .methods(crow::HTTPMethod::GET)
    ([this]()
    {
        try
        {
            const auto urls = database_.getAllUrls();

            crow::json::wvalue::list urlList;

            for (const auto& [shortCode, originalUrl] : urls)
            {
                crow::json::wvalue item;
                item["short_code"] = shortCode;
                item["original_url"] = originalUrl;

                urlList.emplace_back(std::move(item));
            }

            crow::json::wvalue result;
            result["urls"] = std::move(urlList);

            crow::response response(200, result.dump());
            response.set_header("Content-Type", "application/json");
            return response;
        }
        catch (const std::exception& exception)
        {
            std::cerr << "Fetching URLs failed: "
                    << exception.what() << '\n';

            return crow::response(500, "Failed to retrieve URL mappings.");
        }
    });


    // Redirect short URLs using the /r/ prefix for now.
    CROW_ROUTE(app, "/r/<string>")
    ([this](const std::string& shortCode)
    {
        try
        {
            // Retrieve the original URL from PostgreSQL.
            const auto originalUrl = urlService_.resolve(shortCode);

            if (!originalUrl.has_value())
            {
                return crow::response(404, "Short URL not found.");
            }

            // Ask the browser to navigate to the original URL.
            crow::response response(302);
            response.set_header("Location", *originalUrl);
            return response;
        }
        catch (const std::exception& exception)
        {
            std::cerr << "Redirect failed: "
                    << exception.what() << '\n';

            return crow::response(500, "Internal server error.");
        }
    });
}

void UrlShortener::run()
{
    app.port(8080)
       .multithreaded()
       .run();
}