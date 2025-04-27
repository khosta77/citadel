#include "SystemLoadCore.hpp"

SystemLoadCore::SystemLoadCore( const std::string& jsonSetting ) : UniversalServerCore("SystemLoadCore")
{
    systemSetting_ = deserialize<pkg::SystemSetting>(jsonSetting);
}

SystemLoadCore::~SystemLoadCore()
{
    sl_.stop();
}

void SystemLoadCore::Init()
{
    sl_.init(systemSetting_);
}

void SystemLoadCore::Process(const int id, const std::string& name, const std::string& msg )
{
    (void)name;
    pkg::Message message_in;
    try
    {
        message_in = deserialize<pkg::Message>( msg );
    }
    catch( ... )
    {
        pkg::ImOkay message_err;
        message_err.status = INT_MAX;
        writeToSock( id, serialize( message_err ) );
        return;
    }

    pkg::SystemSetting clearMessage;
    try
    {
        clearMessage = deserialize<pkg::SystemSetting>( message_in.text );
    }
    catch( ... )
    {
        pkg::ImOkay message_err;
        message_err.status = INT_MIN;
        writeToSock( id, serialize( message_err ) );
        return;
    }

    //// Перенастройка
    sl_.init(clearMessage);

    pkg::ImOkay message_out;
    message_out.status = message_in.id;
    writeToSock( id, serialize( message_out ) );
}

void SystemLoadCore::Launch()
{

}

void SystemLoadCore::Stop()
{
    sl_.stop();
}


