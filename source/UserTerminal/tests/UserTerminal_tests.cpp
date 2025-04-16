#include <gtest/gtest.h>

#include "ResultDisplayServer.hpp"

TEST( SimpleTest, BasicAssertion )
{
    EXPECT_EQ( 1, 1 );
}

int main( int argc, char **argv )
{
    testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
