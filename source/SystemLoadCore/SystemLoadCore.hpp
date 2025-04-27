#ifndef SYSTEMLOADCORE_HPP_
#define SYSTEMLOADCORE_HPP_

#include "UniversalServer.hpp"
#include "utils.hpp"

#include "dataframe.hpp"

#include "SystemLoad.hpp"

class SystemLoadCore : public UniversalServerCore, public UniversalServerMethods
{
public:
    SystemLoadCore( const std::string& jsonSetting );
    ~SystemLoadCore() override;

    void Init() override;
    void Process(const int id, const std::string& name, const std::string& msg ) override;
    void Launch() override; 
    void Stop() override;
private:
    pkg::SystemSetting systemSetting_;
    SystemLoad sl_;
};

#endif  // SYSTEMLOADCORE_HPP_
