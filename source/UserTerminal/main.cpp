#include "UserTerminal.hpp"
#include "utils.hpp"

int main( int argc, char* argv[] )
{
    IpFromMainInput address_( argc, argv );
    UserTerminal user_( address_.ip_, address_.port_, "User" );
    return user_.run( std::cin );
}
