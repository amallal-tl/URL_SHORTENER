#include "url_shortener/url_service.h"

#include <pqxx/pqxx>

// Called when UrlShortener initializes its urlService_ member.
UrlService::UrlService(Database database) : database_(std::move(database)){
    // Stores the supplied Database dependency for future service operations.
}

std::string UrlService::shorten(const std::string& url){
    while(true){
        // Generate a candidate short code.

        std::string code = generator_.generate();
        try{
            database_.saveUrl(code, url);
            
            return code;
        } catch(const pqxx::unique_violation&) {
            // Another URL already uses this code; generate another one.
        }
    }
}

std::optional<std::string> UrlService::resolve(const std::string& shortCode) const{
    return database_.findUrl(shortCode);
}