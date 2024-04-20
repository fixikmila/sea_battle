#include <boost/chrono.hpp>
#include <iostream>

#include "../Messages/msg_export.h"
#include "../Logic/ServerLogic.h"

int main()
{
    std::cout << "[LOG] Starting server" << std::endl;
    ServerLogic server;

    std::cout << "[LOG] Starting message dispatch loop.." << std::endl;

    std::map<std::pair<std::string, unsigned short>, boost::chrono::system_clock::time_point> timestamps = {};

    while (server.isGood())
    {
        server.DispatchMessage(timestamps);
    }

    std::cout << "[LOG] exiting" << std::endl;
    return 0;
}