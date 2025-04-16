#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <regex>

class IpFromMainInput
{
private:
    void checkIp();

public:
    std::string ip_;
    int port_;

    IpFromMainInput() = delete;
    IpFromMainInput( int argc, char* argv[] );
    ~IpFromMainInput();
};

#endif  // UTILS_HPP_
