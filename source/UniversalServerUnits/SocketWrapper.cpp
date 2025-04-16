#include "SocketWrapper.hpp"

size_t SocketWrapper::write( int fd, const void* buf, size_t count )
{
    return ::write( fd, buf, count );
}

size_t SocketWrapper::read( int fd, void* buf, size_t count )
{
    return ::read( fd, buf, count );
}

