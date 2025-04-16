#ifndef SOCKETWRAPPER_HPP_
#define SOCKETWRAPPER_HPP_

#include "SocketMock.hpp"

#include <unistd.h>

class SocketWrapper : public SocketMock
{
public:
    size_t write( int fd, const void* buf, size_t count ) override;
    size_t read( int fd, void* buf, size_t count ) override;
};

#endif  // SOCKETWRAPPER_HPP_
