#include "utils.hpp"

#include <gtest/gtest.h>

TEST( IpFromMainInputTest, ValidInput )
{
    char* argv[] = {
        (char*)"program_name",
        (char*)"192.168.1.1",
        (char*)"8080"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    EXPECT_NO_THROW(
        {
            IpFromMainInput ip( argc, argv );
            EXPECT_EQ( ip.ip_, "192.168.1.1" );
            EXPECT_EQ( ip.port_, 8080 );
        }
    );
}

TEST( IpFromMainInputTest, InvalidNumberOfArguments )
{
    char* argv1[] = {(char*)"program_name"};
    int argc1 = sizeof(argv1) / sizeof(argv1[0]);
    EXPECT_THROW( IpFromMainInput ip( argc1, argv1 ), std::invalid_argument );

    char* argv2[] = {
        (char*)"program_name",
        (char*)"192.168.1.1"
    };
    int argc2 = sizeof(argv2) / sizeof(argv2[0]);
    EXPECT_THROW( IpFromMainInput ip( argc2, argv2 ), std::invalid_argument );

    char* argv3[] = {
        (char*)"program_name",
        (char*)"192.168.1.1",
        (char*)"8080",
        (char*)"extra_arg"
    };
    int argc3 = sizeof(argv3) / sizeof(argv3[0]);
    EXPECT_THROW( IpFromMainInput ip( argc3, argv3 ), std::invalid_argument );
}

TEST( IpFromMainInputTest, InvalidPortFormat )
{
    char* argv[] = {
        (char*)"program_name",
        (char*)"192.168.1.1",
        (char*)"not_a_number"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    EXPECT_THROW( IpFromMainInput ip( argc, argv ), std::invalid_argument );
}

TEST( IpFromMainInputTest, NegativePort )
{
    char* argv[] = {
        (char*)"program_name",
        (char*)"192.168.1.1",
        (char*)"-1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    EXPECT_THROW( IpFromMainInput ip( argc, argv ), std::out_of_range );
}

TEST(IpFromMainInputTest, InvalidIpFormat)
{
    std::vector<std::string> invalidIps = {
        "256.1.1.1",    // значение > 255
        "1.256.1.1",
        "1.1.256.1",
        "1.1.1.256",
        "1.1.1.",       // неполный адрес
        "1.1.1",        // неполный адрес
        "1.1.1.1.1",    // слишком много октетов
        "a.b.c.d",      // не числа
        "1.1.1.1 ",     // пробел в конце
        " 1.1.1.1",     // пробел в начале
        "1 .1.1.1",     // пробел внутри
        "1.1.1.1\n",    // управляющий символ
        ""              // пустая строка
    };

    for( const auto& ip : invalidIps )
    {
        char* argv[] = {
            (char*)"program_name",
            const_cast<char*>(ip.c_str()),
            (char*)"8080"
        };
        int argc = sizeof(argv) / sizeof(argv[0]);

        EXPECT_THROW( IpFromMainInput ip( argc, argv ), std::invalid_argument )  << "Failed for IP: " << ip;
    }
}

TEST( IpFromMainInputTest, ValidBoundaryIpValues )
{
    std::vector<std::string> validIps = {
        "0.0.0.0",
        "255.255.255.255",
        "1.1.1.1",
        "127.0.0.1",
        "192.168.0.1",
        "10.0.0.1",
        "255.0.0.0",
        "0.255.0.0",
        "0.0.255.0",
        "0.0.0.255"
    };

    for( const auto& ip : validIps )
    {
        char* argv[] = {
            (char*)"program_name",
            const_cast<char*>(ip.c_str()),
            (char*)"8080"
        };
        int argc = sizeof(argv) / sizeof(argv[0]);

        EXPECT_NO_THROW( IpFromMainInput ip( argc, argv ) )  << "Failed for IP: " << ip;
    }
}

TEST( IpFromMainInputTest, ValidBoundaryPortValues )
{
    char* argv1[] = {
        (char*)"program_name",
        (char*)"192.168.1.1",
        (char*)"0"
    };
    int argc1 = sizeof(argv1) / sizeof(argv1[0]);
    EXPECT_NO_THROW( IpFromMainInput ip( argc1, argv1 ) );

    char* argv2[] = {
        (char*)"program_name",
        (char*)"192.168.1.1",
        (char*)"65535" // Максимальный порт
    };
    int argc2 = sizeof(argv2) / sizeof(argv2[0]);
    EXPECT_NO_THROW( IpFromMainInput ip( argc2, argv2 ) );
}

int main( int argc, char **argv )
{
    testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
