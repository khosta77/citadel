#ifndef SOCKETMOCK_HPP_
#define SOCKETMOCK_HPP_

#include <cstdlib>

class SocketMock
{
public:
    virtual ~SocketMock() = default;
    virtual size_t write( int fd, const void* buf, size_t count ) = 0;
    virtual size_t read( int fd, void* buf, size_t count ) = 0;
};

#endif  // SOCKETMOCK_HPP_
