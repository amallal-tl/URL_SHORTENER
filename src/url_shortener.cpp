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

    CROW_ROUTE(app, "/testShorten").methods(crow::HTTPMethod::POST)([this](){
        try{
            const std::string code = urlService_.shorten("http://google.com");
            return crow::response(201, code);
        } catch(const std::exception& exception) {
            std::cerr << "Shorten failed!" << exception.what() <<std::endl;
            return crow::response(404, "Failed to shorten Url!");
        }
    });

    CROW_ROUTE(app, "/testResolve/<string>")([this](const std::string& shortCode){
        const auto originalUrl = urlService_.resolve(shortCode);
        
        if(!originalUrl.has_value()){
            return crow::response(404, "Short code not found!");
        } 
        
        return crow::response(200, *originalUrl);
    });

    CROW_ROUTE(app, "/urls").methods(crow::HTTPMethod::POST)([this](const crow::request& request){
        //Parse the incoming json request body
        const auto body = crow::json::load(request.body);

        if(!body)
            return crow::response(404, "Invalid json body");

        //Ensure required URL field exist
        if(!body.has("url")){
            return crow::response(404, "Missing Url field"); 
        }

        try{
            const std::string originalUrl = body["url"].s();
            const std::string code = urlService_.shorten(originalUrl);//Shortening and inserting into database

            crow::json::wvalue result;
            result["short_code"] = code;
            result["short_url"] = "http://localhost:8080/" + code;

            // Return HTTP 201 Created with the generated URL.
            crow::response response(201, result.dump());
            response.set_header("Content-Type", "application/json");
            return response;
        } catch(const std::exception& exception) {
            std::cerr << "URL creation failed: "
                      << exception.what() << '\n';

            return crow::response(500, "Failed to create short URL.");
        }
    });


    // Redirect a short code to its original URL.
    CROW_ROUTE(app, "/<string>")
    ([this](const std::string& shortCode)
    {
        try
        {
            // Find the original URL using the existing service.
            const auto originalUrl = urlService_.resolve(shortCode);

            // Return 404 if this code does not exist.
            if (!originalUrl.has_value())
            {
                return crow::response(404, "Short URL not found.");
            }

            // Tell the browser where to navigate next.
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