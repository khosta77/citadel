#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <iostream>
#include <vector>
#include <string>
#include <format>
#include <cstdint>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <deque>
#include <functional>
#include <exception>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

#include "UniversalServerUnits.hpp"

class UniversalServer : protected UniversalServerMethods
{
private:
    const int MAX_CLIENTS = 1024;

    std::unique_ptr<int[]> clients_fds_ = std::make_unique<int[]>( MAX_CLIENTS );
    std::unique_ptr<struct pollfd[]> fds_ = std::make_unique<struct pollfd[]>( ( MAX_CLIENTS + 1 ) );

    std::unordered_map<int, std::string> clients_name_;

    std::atomic<bool> serverWorkStatus_;

    std::string ip_;
    int port_;
    int server_fd_;
    int nfds_ = 1;

    struct sockaddr_in server_addr_;
    struct sockaddr_in client_addr_;
    socklen_t client_len_ = sizeof(client_addr_);

    std::unique_ptr<UniversalServerCore> core_;

    /*
     *  @brief
     * */
    void launchServer();

    /*
     * @brief Настройка файлогового дескриптора
     * */
    void settingsFileDescriptor();

    /*
     * @brief Проверяет новые подключения к серверу
     * */
    void checkingSocketsOnNewConnect();

    /*
     * @brief Метод для закрытия сокетов, если сообщения пустые
     * */
    bool ifMessageEmptyCloseSocket( const int );

    /*
     * @brief надо проверить при первом подключении, что, тот с кем хотим работать имеет имя
     * */
    bool get_WhoAmI_Info( const int, std::string& );

    /*
     * @brief когда проверки доходят до этого метода можно быть увереным, что у нас 'сообщение'
     *
     * Сообщение отправится в процесс ядра, что будет в ядре уже остается в правах ядра.
     *
     * * Ядро вправе отправить пользователю, сообщения с информацией и т.д., но может и не отправлять.
     *
     * Декодирование сообщения соответсвенно происходит в ядре.
     * */
    void processTheRequest( const int, std::string& );

    /*
     * @brief checkingSocketsOnNewContent - проверяет какие сообщения пришли от сокетов пользователей
     * */
    void checkingSocketsOnNewContent();

public:
    UniversalServer( const std::string&, const int&,  std::unique_ptr<UniversalServerCore> );
    UniversalServer( const UniversalServer& ) = delete;
    UniversalServer( UniversalServer&& ) = delete;
    UniversalServer& operator=( const UniversalServer& ) = delete;
    UniversalServer& operator=( UniversalServer&& ) = delete;
    ~UniversalServer();

    /*
     * @brief запустить сервер. !!! Блокирует поток !!!
     * */
    int run();

    /*
     * @brief остановить сервер, без возможности повторного запуска
     * */
    void stop();
};

#endif  // SERVER_HPP_
