#pragma once

#include "url_shortener/short_code_generator.h"
#include "url_shortener/database.h"

#include <optional>
#include <string>
#include <unordered_map>

class UrlService{
    public:
        explicit UrlService(Database database);
        std::string shorten(const std::string& url);
        std::optional<std::string> resolve(
            const std::string& shortcode) const;

    private:
        ShortCodeGenerator generator_;
        std::unordered_map<std::string, std::string> urls_;
};