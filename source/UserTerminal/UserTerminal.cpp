#include "UserTerminal.hpp"

int UserTerminal::getRandomNumber()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist;
    return dist(gen);
}

void UserTerminal::socketInit()
{
    sock_ = socket( AF_INET, SOCK_STREAM, 0 );
    if( sock_ < 0 )
        throw std::runtime_error( ( "listener = " + std::to_string(sock_) ) );
}

void UserTerminal::fillAddress( const std::string& IP, const int& PORT )
{
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP.c_str());
}

void UserTerminal::connectInit()
{
    if( connect( sock_, ( struct sockaddr* )&addr, sizeof(addr) ) < 0 )
        throw std::runtime_error( "connect" );
}

void UserTerminal::buildBridge()
{
    pkg::WhoWantsToTalkToMe wwtttm_;
    wwtttm_.name = ( my_name_ + std::to_string( getRandomNumber() ) );
    writeToSock( sock_, serialize( wwtttm_ ) );
}

std::string UserTerminal::packMessage( const uint32_t id, const std::string& logFrame )
{
    pkg::Message message_;
    message_.id = id;
    message_.text = logFrame;
    return serialize( message_ );
}

UserTerminal::UserTerminal( const std::string& IP, const int& PORT, const std::string& MY_NAME ) :
    UniversalServerMethods(), my_name_(MY_NAME)
{
    socketInit();
    fillAddress( IP, PORT );
    connectInit();
    buildBridge();
}

UserTerminal::~UserTerminal()
{
    close(sock_);
}

int UserTerminal::run( std::istream& in )
{
    while( true )
    {
        std::string msg;
        std::cout << ">:";
        std::getline( in, msg );
        const int id = getRandomNumber();
        writeToSock( sock_, packMessage( id, msg ) );
    
        std::string message_from = readFromSock( sock_ );
        std::vector<std::string> messages = split( std::move( message_from ) );
        
        if( messages.size() != 1 )
            throw std::runtime_error( "Extra parcels from server" );

        pkg::ImOkay status_;
        status_ = deserialize<pkg::ImOkay>( messages[0] );

        if( status_.status != id )
            throw std::runtime_error( "Incorrect value from server" );
    }
    return 0;
}
