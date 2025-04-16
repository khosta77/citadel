#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <exception>
#include <format>
#include <string>

class MyException : public std::exception
{
public:
    explicit MyException( const std::string& msg );
    
    const char *what() const noexcept override;

private:
    std::string m_msg;
};

class ErrorReadingFromSocket : public MyException
{
public:
    const int sock;

    ErrorReadingFromSocket( const int socket_ );
};

class NotCorrectMessageToSend : public MyException
{
public:
    NotCorrectMessageToSend();
};

class ErrorWritingToSocket : public MyException
{
public:
    const int sock;

    ErrorWritingToSocket( const int socket_ );
};

class DeserializeJsonNoKey : public MyException
{
public:
    DeserializeJsonNoKey( const std::string& name ) : MyException( std::format(
        "In Deserialize json problem with {}", name
    ) ) {}    
};

class DeserializeJsonElementSomeProblem : public MyException
{
public:
    DeserializeJsonElementSomeProblem( const std::string& name ) : MyException( std::format(
        "In Deserialize json problem with element {}", name
    ) ) {}
};

class SocketNotCreate : public MyException
{
public:
    SocketNotCreate() : MyException("Socket not create") {}
};

class BindFailure : public MyException
{
public:
    BindFailure() : MyException("bind some problem") {}
};

class ListenException : public MyException
{
public:
    const int code;
    ListenException( const int code_ ) : MyException("Listen error"), code(code_) {}
};

class POLLDestroyed : public MyException
{
public:
    POLLDestroyed() : MyException("poll -") {}
};

#endif  // EXCEPTION_HPP_
