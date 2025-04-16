#include "UniversalServer.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <future>

//// UniversalServer

// Что-то все порты заняты((((
const int TEST_PORT = 85201;

class Writer : public UniversalServerMethods
{
private:
    const std::string my_name_;

    int sock_;
    struct sockaddr_in addr;

    void socketInit()
    {
        sock_ = socket( AF_INET, SOCK_STREAM, 0 );
        if( sock_ < 0 )
            throw std::runtime_error( ( "listener = " + std::to_string(sock_) ) );
    }

    void fillAddress( const std::string& IP, const int& PORT )
    {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
        addr.sin_addr.s_addr = inet_addr(IP.c_str());
    }

    void connectInit()
    {
        if( connect( sock_, ( struct sockaddr* )&addr, sizeof(addr) ) < 0 )
            throw std::runtime_error( "connect" );
    }

    void buildBridge()
    {
        pkg::WhoWantsToTalkToMe wwtttm_;
        wwtttm_.name = my_name_;
        writeToSock( sock_, serialize( wwtttm_ ) );
    }

    std::string packMessage( const uint32_t id, const std::string& logFrame )
    {
        pkg::Message message_;
        message_.id = id;
        message_.text = logFrame;
        return serialize( message_ );
    }

public:
    Writer( const std::string& IP = "127.0.0.1", const int& PORT = TEST_PORT,
        const std::string& MY_NAME = "Tester" ) : UniversalServerMethods(), my_name_(MY_NAME)
    {
        socketInit();
        fillAddress( IP, PORT );
        connectInit();
        buildBridge();
    }
    
    ~Writer()
    {
        close(sock_);
    }

    void write( const std::string& text )
    {
        const std::string serverMessage = packMessage( 200, text );
        writeToSock( sock_, serverMessage );
    }
};

class MockCore : public UniversalServerCore
{
public:
    MockCore() : UniversalServerCore("MockCore") {}
    ~MockCore() override = default;

    void Init() override {}
    MOCK_METHOD(void, Process, (const int, const std::string&, const std::string&), (override));
    void Launch() override {}
    void Stop() override {}
};


TEST( UniversalServerTest, MethodCalls )
{
    auto mockCore = std::make_unique<MockCore>();
    
    MockCore* mockPtr = mockCore.get();
    EXPECT_CALL( *mockPtr, Process( testing::_, testing::_, testing::_ ) ).Times(1);

    UniversalServer server( "127.0.0.1", TEST_PORT, std::move( mockCore ) );
    std::future<int> server_status = std::async( std::launch::async, [&](){ return server.run(); } );
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    Writer user_( "127.0.0.1", TEST_PORT, "user" );
    user_.write( "123456789" );
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    server.stop();
    const int status = server_status.get();
    EXPECT_EQ( status, 0 );

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST( UniversalServerTest, MethodCallsAndClose )
{
    auto mockCore = std::make_unique<MockCore>();

    MockCore* mockPtr = mockCore.get();
    EXPECT_CALL( *mockPtr, Process( testing::_, testing::_, testing::_ ) ).Times(1);

    UniversalServer server( "127.0.0.1", TEST_PORT, std::move( mockCore ) );
    std::future<int> server_status = std::async( std::launch::async, [&](){ return server.run(); } );
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    {
        Writer user_( "127.0.0.1", TEST_PORT, "user" );
        user_.write( "123456789" );
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    server.stop();
    const int status = server_status.get();
    EXPECT_EQ( status, 0 );

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int main( int argc, char **argv )
{
    testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}


