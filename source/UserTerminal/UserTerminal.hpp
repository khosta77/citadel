#ifndef USERTERMINAL_HPP_
#define USERTERMINAL_HPP_

#include "UniversalServer.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <random>

class UserTerminal : public UniversalServerMethods
{
private:
    const std::string my_name_;

    int getRandomNumber();

    int sock_;
    struct sockaddr_in addr;

    void socketInit();
    void fillAddress( const std::string& IP, const int& PORT );
    void connectInit(); 
    void buildBridge();

    std::string packMessage( const uint32_t id, const std::string& logFrame );

public:
    UserTerminal( const std::string& IP, const int& PORT, const std::string& MY_NAME );
    ~UserTerminal();

    int run( std::istream& in );
};

#endif  // USERTERMINAL_HPP_
