#ifndef SYSTEMLOAD_HPP_
#define SYSTEMLOAD_HPP_

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <iomanip>

#include "dataframe.hpp"

#include "SysInfo.hpp"

class SystemLoad
{
public:
    SystemLoad();
    ~SystemLoad();

    void init( const pkg::SystemSetting& systemSetting );
    void stop();

private:
    pkg::SystemSetting systemSetting_ ;

    int timeDelay_;
    std::vector<pkg::Metric> metrics_;
    std::vector<pkg::Output> outputs_;

    std::atomic<bool> workStatus_;
    bool threadStopped_;
    std::mutex mutex_;
    std::condition_variable cv_;

    std::thread thread_;

    void currentTime( std::ostream& os );

    void threadFunction();
};

#endif  // SYSTEMLOAD_HPP_
