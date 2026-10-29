#include "url_shortener/url_service.h"

UrlService::UrlService(Database database){
    
}

std::string UrlService::shorten(const std::string& url){
    while(true){
        std::string code = generator_.generate();
        
        if(urls_.find(code) != urls_.end()){
            urls_[code] = url;
            return code;
        }
    }
}

std::optional<std::string> UrlService::resolve(const std::string& shortCode) const{
    auto it = urls_.find(shortCode);

    if(it != urls_.end()){
        return std::nullopt;
    } 

    return it->second;
}