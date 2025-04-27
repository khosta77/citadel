#ifndef SYSINFO_HPP_
#define SYSINFO_HPP_

#include <iostream>
#include <vector>
#include <string>

std::vector<int> getCPULoad( const std::vector<int>& cores );
std::vector<int> getMemoryLoad( const std::vector<std::string>& memorys );

#endif  // SYSINFO_HPP_
