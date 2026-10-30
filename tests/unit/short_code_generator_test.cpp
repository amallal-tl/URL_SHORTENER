#include <gtest/gtest.h>

#include "url_shortener/short_code_generator.h"

#include <string>
#include <unordered_set>
#include <cctype>

TEST(ShortCodeGeneratorTest, GeneratedCodeHasSixCharacter)
{
    ShortCodeGenerator generator;

    const std::string code = generator.generate();

    EXPECT_EQ(code.length(), 6);
}

TEST(ShortCodeGeneratorTest, GeneratedCodeContainsOnlyAllowedCharacters)
{
    ShortCodeGenerator generator;

    const std::string allowedCharacters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    const std::string code = generator.generate();

    for(const char character : code)
    {
        EXPECT_NE(allowedCharacters.find(character), std::string::npos);
    }
}

TEST(ShortCodeGeneratorTest, GeneratesMultipleValidCodes)
{
    ShortCodeGenerator generator;

    constexpr int numberOfCodes = 100;

    for (int i = 0; i < numberOfCodes; ++i)
    {
        const std::string code = generator.generate();

        EXPECT_EQ(code.length(), 6);

        for (const char character : code)
        {
            EXPECT_TRUE(
                std::isalnum(static_cast<unsigned char>(character))
            );
        }
    }
}