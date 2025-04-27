#include "SysInfo.hpp"

#include <mach/mach.h>
#include <mach/mach_host.h>
#include <mach/vm_statistics.h>
#include <mach/processor_info.h>
#include <sys/sysctl.h>
#include <unistd.h>

std::vector<int> getCPULoad( const std::vector<int>& cores )
{
    std::vector<int> loads;
    natural_t numProcessors = 0;
    processor_info_array_t infoArray1, infoArray2;
    mach_msg_type_number_t infoCount;

    if( host_processor_info( mach_host_self(), PROCESSOR_CPU_LOAD_INFO, &numProcessors, &infoArray1,
        &infoCount ) != KERN_SUCCESS )
    {
        return {};
    }

    usleep(500000);

    if( host_processor_info( mach_host_self(), PROCESSOR_CPU_LOAD_INFO, &numProcessors, &infoArray2,
        &infoCount ) != KERN_SUCCESS )
    {
        vm_deallocate( mach_task_self(), (vm_address_t)infoArray1, infoCount );
        return {};
    }

    for( int core : cores )
    {
        if( ( ( core < 0 ) or ( static_cast<natural_t>(core) >= numProcessors ) ) )
        {
            loads.push_back(-1);
            continue;
        }

        auto& load1 = ( (processor_cpu_load_info_t)infoArray1 )[core];
        auto& load2 = ( (processor_cpu_load_info_t)infoArray2 )[core];

        unsigned long totalTicks =
            ( load2.cpu_ticks[CPU_STATE_USER] - load1.cpu_ticks[CPU_STATE_USER] ) +
            ( load2.cpu_ticks[CPU_STATE_SYSTEM] - load1.cpu_ticks[CPU_STATE_SYSTEM] ) +
            ( load2.cpu_ticks[CPU_STATE_IDLE] - load1.cpu_ticks[CPU_STATE_IDLE] ) +
            ( load2.cpu_ticks[CPU_STATE_NICE] - load1.cpu_ticks[CPU_STATE_NICE] );

        unsigned long usedTicks =
            ( load2.cpu_ticks[CPU_STATE_USER] - load1.cpu_ticks[CPU_STATE_USER] ) +
            ( load2.cpu_ticks[CPU_STATE_SYSTEM] - load1.cpu_ticks[CPU_STATE_SYSTEM] ) +
            ( load2.cpu_ticks[CPU_STATE_NICE] - load1.cpu_ticks[CPU_STATE_NICE] );

        loads.push_back( totalTicks == 0 ? 0 : static_cast<int>( ( usedTicks * 100 ) / totalTicks ) );
    }

    vm_deallocate( mach_task_self(), (vm_address_t)infoArray1, infoCount );
    vm_deallocate( mach_task_self(), (vm_address_t)infoArray2, infoCount );
    return loads;
}

std::vector<int> getMemoryLoad( const std::vector<std::string>& memorys )
{
    std::vector<int> results;
    vm_size_t pageSize;
    mach_port_t host = mach_host_self();
    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
    vm_statistics64_data_t vmStats;

    if( host_page_size( host, &pageSize ) != KERN_SUCCESS or
        host_statistics64( host, HOST_VM_INFO64, (host_info64_t)&vmStats, &count ) != KERN_SUCCESS )
    {
        return {};
    }

    int64_t totalMemBytes;
    size_t len = sizeof(totalMemBytes);
    sysctlbyname( "hw.memsize", &totalMemBytes, &len, NULL, 0 );

    // Используемая память (active + wired)
    uint64_t usedMemBytes = ( vmStats.active_count + vmStats.wire_count ) * pageSize;
    // Свободная память (free + inactive)
    uint64_t freeMemBytes = ( vmStats.free_count + vmStats.inactive_count ) * pageSize;

    const uint64_t MB = 1024 * 1024;
    int usedMemMB = static_cast<int>( usedMemBytes / MB );
    int freeMemMB = static_cast<int>( freeMemBytes / MB );

    for( const auto& memType : memorys )
    {
        if( memType == "used" )
        {
            results.push_back(usedMemMB);
        } else if( memType == "free" )
        {
            results.push_back(freeMemMB);
        }
        else
        {
            // TODO: неизвестный
        }
    }

    return results;
}
