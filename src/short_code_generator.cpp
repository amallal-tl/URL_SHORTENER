
#include "url_shortener/short_code_generator.h"

#include <random>
#include <string>

ShortCodeGenerator::ShortCodeGenerator()
    : generator_(std::random_device{}())
{
}

std::string ShortCodeGenerator::generate()
{
    const std::string characters =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::uniform_int_distribution<std::size_t> distribution(
        0, characters.size() - 1);

    std::string code;
    code.reserve(6);

    for (int i = 0; i < 6; ++i)
    {
        const std::size_t index = distribution(generator_);
        code += characters[index];
    }

    return code;
}
