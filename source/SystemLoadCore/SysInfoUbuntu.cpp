#include "SysInfo.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>

// Функция для получения загрузки CPU в процентах для каждого ядра
std::vector<int> getCPULoad(const std::vector<int>& cores) {
    std::vector<int> loadPercentages;

    // Читаем статистику CPU из /proc/stat
    std::ifstream statFile("/proc/stat");
    std::string line;

    // Пропускаем первую строку (общую статистику)
    std::getline(statFile, line);

    // Читаем статистику для каждого ядра
    for (int core : cores) {
        if (core < 0) continue; // Пропускаем неверные номера ядер

        // Ищем строку для запрошенного ядра
        for (int i = 0; i <= core; ++i) {
            if (!std::getline(statFile, line)) {
                // Если ядро не найдено, добавляем 0
                loadPercentages.push_back(0);
                break;
            }
        }

        if (line.find("cpu" + std::to_string(core)) == 0) {
            // Парсим значения времени
            std::istringstream iss(line);
            std::string cpuLabel;
            unsigned long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
            iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

            // Вычисляем общее время работы и время простоя
            unsigned long totalIdle = idle + iowait;
            unsigned long totalNonIdle = user + nice + system + irq + softirq + steal;
            unsigned long total = totalIdle + totalNonIdle;

            // Читаем значения еще раз после небольшой паузы
            usleep(100000); // 100ms
            statFile.clear();
            statFile.seekg(0);

            // Пропускаем строки до нужного ядра
            for (int i = 0; i <= core + 1; ++i) {
                std::getline(statFile, line);
            }

            iss.str(line);
            iss.clear();
            iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

            unsigned long totalIdle2 = idle + iowait;
            unsigned long totalNonIdle2 = user + nice + system + irq + softirq + steal;
            unsigned long total2 = totalIdle2 + totalNonIdle2;

            // Вычисляем загрузку в процентах
            unsigned long totald = total2 - total;
            unsigned long idled = totalIdle2 - totalIdle;

            if (totald == 0) {
                loadPercentages.push_back(0);
            } else {
                int percentage = static_cast<int>((totald - idled) * 100 / totald);
                loadPercentages.push_back(percentage);
            }
        } else {
            loadPercentages.push_back(0);
        }

        statFile.clear();
        statFile.seekg(0);
    }

    return loadPercentages;
}

// Функция для получения информации об использовании памяти
std::vector<int> getMemoryLoad(const std::vector<std::string>& memorys) {
    std::vector<int> memoryInfo;

    // Читаем информацию о памяти из /proc/meminfo
    std::ifstream memFile("/proc/meminfo");
    std::string line;

    unsigned long totalMem = 0;
    unsigned long freeMem = 0;
    unsigned long availableMem = 0;
    unsigned long buffers = 0;
    unsigned long cached = 0;

    while (std::getline(memFile, line)) {
        if (line.find("MemTotal:") == 0) {
            std::istringstream iss(line.substr(9));
            iss >> totalMem;
        } else if (line.find("MemFree:") == 0) {
            std::istringstream iss(line.substr(8));
            iss >> freeMem;
        } else if (line.find("MemAvailable:") == 0) {
            std::istringstream iss(line.substr(12));
            iss >> availableMem;
        } else if (line.find("Buffers:") == 0) {
            std::istringstream iss(line.substr(8));
            iss >> buffers;
        } else if (line.find("Cached:") == 0) {
            std::istringstream iss(line.substr(7));
            iss >> cached;
        }
    }

    // Вычисляем использованную память
    unsigned long usedMem = totalMem - freeMem - buffers - cached;

    // Формируем результат в зависимости от запрошенных данных
    for (const auto& memType : memorys) {
        if (memType == "used") {
            memoryInfo.push_back( usedMem / 1024 );
        } else if (memType == "free") {
            memoryInfo.push_back( freeMem / 1024 );
        }
        else {
            // TODO: неизвестно
        }
    }

    return memoryInfo;
}

