#pragma once

#include "crow.h"
#include "url_shortener/url_service.h"
#include "url_shortener/database.h"

class UrlShortener{
    public:
        UrlShortener();
        void run();

    private:
        crow::SimpleApp app;
        Database database_;
        UrlService urlService_{database_};
        void registerRoutes();
};