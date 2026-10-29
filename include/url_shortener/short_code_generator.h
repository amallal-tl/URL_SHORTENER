#pragma once

#include <random>
#include <string>

class ShortCodeGenerator{
    public:
        ShortCodeGenerator();
    
        std::string generate();

    private:
        std::mt19937 generator_;
};