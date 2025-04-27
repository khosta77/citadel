#include "SystemLoad.hpp"

SystemLoad::SystemLoad() : workStatus_(false), threadStopped_(false) {}

SystemLoad::~SystemLoad()
{
    if( workStatus_ )
    {
        stop();
        if( thread_.joinable() )
            thread_.join();
    }
}

void SystemLoad::init( const pkg::SystemSetting& systemSetting )
{
    if( workStatus_ )
    {
        stop();
        if( thread_.joinable() )
            thread_.join();
    }

    systemSetting_ = systemSetting;

    timeDelay_ = systemSetting_.setting.period;
    metrics_ = systemSetting_.metrics;
    outputs_ = systemSetting_.outputs;

    thread_ = std::thread( &SystemLoad::threadFunction, this );
}

void SystemLoad::stop()
{
    if( workStatus_ )
    {
        workStatus_ = false;
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait( lock, [&](){ return threadStopped_; } );
        threadStopped_ = false;
    }
}

void SystemLoad::currentTime( std::ostream& os )
{
    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    os << "["
       << std::setfill('0') << std::setw(2) << local_time->tm_mday << "-"
       << std::setfill('0') << std::setw(2) << ( local_time->tm_mon + 1 ) << "-"
       << std::setfill('0') << std::setw(4) << ( local_time->tm_year + 1900 ) << " "
       << std::setfill('0') << std::setw(2) << local_time->tm_hour << ":"
       << std::setfill('0') << std::setw(2) << local_time->tm_min << ":"
       << std::setfill('0') << std::setw(2) << local_time->tm_sec
       << "]";
}

void SystemLoad::threadFunction()
{
    workStatus_ = true;
    while( workStatus_ )
    {
        //// Считываем метрики
        pkg::SystemInfo sysInfos_;
        for( const auto& metric_ : metrics_ )
        {
            if( metric_.type == "cpu" )
            {
                sysInfos_.cpu = getCPULoad( metric_.ids );
            }
            else if( metric_.type == "memory" )
            {
                sysInfos_.memory = getMemoryLoad( metric_.spec );
            }
            else
            {
                // TODO: если не корректную метрику отправили
            }
        }

        //// Выводим
        for( const auto& output_ : outputs_ )
        {
            if( output_.type == "console" )
            {
                currentTime(std::cout);
                std::cout << " CPU:";
                for( const auto& it : sysInfos_.cpu )
                {
                    std::cout << '\t' << it;
                }
                std::cout << "\n\r";
                currentTime(std::cout);
                std::cout << " MEMORY:";
                for( const auto& it : sysInfos_.memory )
                {
                    std::cout << '\t' << it;
                }
                std::cout << "\n\r" << std::endl;
            }
            else if( output_.type == "log" )
            {
                // TODO: Не успеваю по времени, не будет реализовано
            }
            else if( output_.type == "service" )
            {
                // TODO: Не успеваю по времени, не будет реализовано
            }
            else
            {
                // TODO: если не корректную метрику отправили
            }
        }

        std::this_thread::sleep_for( std::chrono::seconds( timeDelay_ ) );
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        threadStopped_ = true;
    }
    cv_.notify_one();
}


