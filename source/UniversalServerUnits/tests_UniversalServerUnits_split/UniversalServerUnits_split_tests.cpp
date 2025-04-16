#include "UniversalServer.hpp"

#include <gtest/gtest.h>

//// split

TEST( splitTest, EmptyString )
{
    UniversalServerMethods serverMethods;
    const std::string input = "";
    std::vector<std::string> expected = {};
    auto result = serverMethods.split(input);
    EXPECT_EQ(result, expected);
}

TEST( splitTest, NoDelimiters )
{
    UniversalServerMethods serverMethods;
    const std::string input = "single message";
    std::vector<std::string> expected = { "single message" };
    auto result = serverMethods.split(input);
    EXPECT_EQ( result, expected );
}

TEST( splitTest, SingleDelimiterAtEnd )
{
    UniversalServerMethods serverMethods;
    const std::string input = "message\n\n";
    std::vector<std::string> expected = { "message" };
    auto result = serverMethods.split(input);
    EXPECT_EQ(result, expected);
}

TEST( splitTest, SingleDelimiterInMiddle )
{
    UniversalServerMethods serverMethods;
    const std::string input = "first\n\nsecond";
    std::vector<std::string> expected = {"first", "second"};
    auto result = serverMethods.split(input);
    EXPECT_EQ( result, expected );
}

TEST( splitTest, MultipleDelimiters )
{
    UniversalServerMethods serverMethods;
    const std::string input = "first\n\nsecond\n\nthird";
    std::vector<std::string> expected = { "first", "second", "third" };
    auto result = serverMethods.split(input);
    EXPECT_EQ( result, expected );
}

TEST( splitTest, ConsecutiveDelimiters )
{
    UniversalServerMethods serverMethods;
    const std::string input = "first\n\n\n\nsecond";
    std::vector<std::string> expected = { "first", "second" };
    auto result = serverMethods.split(input);
    EXPECT_EQ( result, expected );
}

TEST( splitTest, DelimiterAtStart )
{
    UniversalServerMethods serverMethods;
    const std::string input = "\n\nmessage";
    std::vector<std::string> expected = { "message" };
    auto result = serverMethods.split(input);
    EXPECT_EQ( result, expected );
}

TEST( splitTest, OnlyDelimiters )
{
    UniversalServerMethods serverMethods;
    const std::string input = "\n\n\n\n";
    std::vector<std::string> expected = {};
    auto result = serverMethods.split(input);
    EXPECT_EQ( result, expected );
}

TEST( splitTest, OneDelimiters )
{
    UniversalServerMethods serverMethods;
    const std::string input = "\n\n\n";
    std::vector<std::string> expected = { "\n" };
    auto result = serverMethods.split(input);
    EXPECT_EQ( result, expected );
}

int main( int argc, char **argv )
{
    testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}


