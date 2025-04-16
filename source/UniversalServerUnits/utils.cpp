#include "utils.hpp"

void IpFromMainInput::checkIp()
{
    std::regex ipv4(
        "(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");
    if( !std::regex_match( ip_, ipv4 ) )
        throw std::invalid_argument( "Invalid IPv4" );
}

IpFromMainInput::IpFromMainInput( int argc, char* argv[] )
{
    if( argc != 3 )
        throw std::invalid_argument( "Usage: program <ip_address> <port>" );

    ip_ = argv[1];

    try
    {
        port_ = std::stoi( argv[2] );
    }
    catch( const std::exception& e )
    {
        throw std::invalid_argument( "Port must be a valid integer" );
    }

    if( port_ < 0 )
        throw std::out_of_range( "Port must be >= 0" );

    checkIp();
}

IpFromMainInput::~IpFromMainInput()
{
    ip_.clear();
}


