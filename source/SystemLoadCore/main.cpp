#include "SystemLoadCore.hpp"

const std::string globalJsonSetting = R"""(
{
    "setting" : {
        "period" : 1
    },
    "metrics" : [
        {
            "type" : "cpu",
            "ids" : [ 0, 1, 2, 3, 4 ],
            "spec" : []
        },
        {
            "type":"memory",
            "ids" : [],
            "spec" : [ "used", "free" ]
        }
    ],
    "outputs" : [
        {
            "type" : "console",
            "ip" : "",
            "path" : "",
            "port" : 0
        },
        {
            "type" : "log",
            "ip" : "",
            "path" : "/path/to/file.log",
            "port" : 0
        },
        {
            "type" : "service",
            "ip" : "127.0.0.1",
            "path" : "",
            "port" : 34000
        }
    ]
}
)""";

int main( [[maybe_unused]] int argc, [[maybe_unused]] char* argv[] )
{
    std::unique_ptr<SystemLoadCore> core_ = std::make_unique<SystemLoadCore>( globalJsonSetting );
    
    if( argc == 1 )
    {
        core_->Init();
        for(;;);
        return 0;
    }
    
    IpFromMainInput address_( argc, argv );
    UniversalServer server_( address_.ip_, address_.port_, std::move( core_ ) );
    return server_.run();
}

