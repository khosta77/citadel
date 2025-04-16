#include "Exceptions.hpp"

MyException::MyException( const std::string& msg ) : m_msg(msg) {}

const char *MyException::what() const noexcept 
{
    return m_msg.c_str();
}

ErrorReadingFromSocket::ErrorReadingFromSocket( const int socket_ ) : MyException(
        "In \'UniversalServerMethods::readFromSock\', something error" ), sock(socket_) {}

NotCorrectMessageToSend::NotCorrectMessageToSend() : MyException( "Not correct message to send" ) {}

ErrorWritingToSocket::ErrorWritingToSocket( const int socket_ ) : MyException(
        "In \'UniversalServerMethods::writeToSock\', something error" ), sock(socket_) {}
