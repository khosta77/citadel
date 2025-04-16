#include "UniversalServerUnits.hpp"

UniversalServerMethods::UniversalServerMethods() : MAX_BUFFER_COUNT(1024),
    socketMock_( std::make_unique<SocketWrapper>() ) {}

UniversalServerMethods::UniversalServerMethods( std::unique_ptr<SocketMock> socketmock ) :
    MAX_BUFFER_COUNT(1024), socketMock_( std::move( socketmock ) ) {}

std::string UniversalServerMethods::readFromSock( const int socket_ ) 
{
    std::string rxData;
    std::vector<char> buffer( MAX_BUFFER_COUNT, 0 );

    while(true)
    {
        int bytesReceived = socketMock_->read( socket_, buffer.data(), MAX_BUFFER_COUNT );
        if( bytesReceived == 0 )
        {
            // Просто выходит, посылка получена, значит
            break;
        }

        if( bytesReceived < 0 )
            throw ErrorReadingFromSocket( socket_ );

        rxData.append( buffer.data(), bytesReceived );

        if( ( ( rxData.back() == '\n' ) and ( *( rxData.rbegin() + 1 ) == '\n' ) ) )
        {
            // Посылка закончилась, выход
            break;
        }
    }

    buffer.clear();
    return rxData;
}

void UniversalServerMethods::writeToSock( const int socket_, std::string msg )
{
    if( msg.find( "\n\n" ) != std::string::npos )
        throw NotCorrectMessageToSend();

    msg += "\n\n";
    const char* dataPtr = msg.c_str();
    size_t dataSize = msg.length();
    size_t totalSend = 0;

    while( totalSend < dataSize )
    {
        int bytesSend = socketMock_->write( socket_, ( dataPtr + totalSend ), ( dataSize - totalSend ) );
        if( bytesSend == -1 )
            throw ErrorWritingToSocket(socket_);

        totalSend += bytesSend;
    }
}

std::vector<std::string> UniversalServerMethods::split( const std::string& message )
{
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = message.find("\n\n");
    
    while( end != std::string::npos )
    {
        result.push_back( message.substr( start, end - start ) );
        if( result.back().empty() )
            result.pop_back();
        start = end + 2;
        end = message.find( "\n\n", start );
    }

    if( start < message.length() )
        result.push_back( message.substr(start) );

    return result;
}
