#ifndef UNIVERSALSERVERCORE_HPP_
#define UNIVERSALSERVERCORE_HPP_

/*
 * @class код для сервера выходит одинаковым во много, для того, чтобы не писать и переписывать например
 *        инициализацию по много раз проще написать ядро указатель на который буду передавать в сервер
 * */
class UniversalServerCore
{
private:

protected:

public:
    std::string serverName_;

    UniversalServerCore( const std::string serverName ) : serverName_(serverName) {}
    /*
     * @brief чтобы не забыли
     * */
    virtual ~UniversalServerCore() = default;

    /*
     * @brief так как решение универсальное, то в некоторых сценариях возможно нужна будет инициализация
     * */
    virtual void Init() = 0;

    /*
     * @brief метод для того чтобы вызвать при подтверждении приема сообщения
     * @param 1 - сокет
     * @param 2 - имя пользователя ( возможно использовании во внутренней логики )
     * @param 3 - сообщение, это закодированная строка, в ней может быть все что угодно.
     * */
    virtual void Process( const int, const std::string&, const std::string& ) = 0;

    /*
     * @brief запуск каго-то внутреннего действия
     * */
    virtual void Launch() = 0;

    /*
     * @brief остановка какого-то внутреннего действия
     * */
    virtual void Stop() = 0;
};

#endif  // UNIVERSALSERVERCORE_HPP_
