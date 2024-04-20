//
// Created by milana on 15.04.24.
//

#ifndef SHIPS_SERVERLOGIC_H
#define SHIPS_SERVERLOGIC_H
#include <vector>
#include <map>
#include <string>
#include "../Utils/Config.h"
#include "../Messages/msg.h"
#include "../Networking/Agent.h"

class ServerLogic {
private:
    Agent *agent;
public:
    ServerLogic();
    ~ServerLogic();
    bool isGood();
    void DispatchMessage(std::map<std::pair<std::string, unsigned short>, boost::chrono::system_clock::time_point>& timestamps);

    //ServerLogic();
    //void DispatchMessage();
    //~ServerLogic();
};


#endif //SHIPS_SERVERLOGIC_H
