//
// Created by milana on 15.04.24.
//

#include "ClientLogic.h"
#include <iostream>
ClientLogic::ClientLogic() {
    agent = new Agent();
    bool s = agent->start(CLIENT_PORT_RANGE_START, CLIENT_PORT_RANGE_END);
    if (!s)
    {
        std::cout << "[ERR] Error while starting networking" << std::endl;
        //return 1;
    }
}
bool ClientLogic::isGood(){
    return agent->isWorking && !agent->isFailed;
}
void ClientLogic::DispatchMessage(std::map<std::pair<std::string, unsigned short>, std::vector<GLfloat>>&data, GLfloat x, GLfloat y, GLfloat z){
    msg = agent->getMessage();
    if (msg)
    {
        switch (msg->Type)
        {
            case Messages::ServerAliveBroadcast:
            {
                srvaddr = msg->AddressFrom;
                isSrvFound = true;
            }
                break;
            case Messages::ClientDataPropagation:
            {
                auto cd_msg = dynamic_cast<Messages::ClientDataPropagationMessage*>(msg);
                data.erase(cd_msg->Id);
                data.insert(std::pair<std::pair<std::string, unsigned short>, std::vector<GLfloat>>(cd_msg->Id, cd_msg->Data));
            }
                break;
            case Messages::ClientRemoval:
            {
                auto cd_msg = dynamic_cast<Messages::ClientRemovalMessage*>(msg);
                data.erase(cd_msg->Id);
            }
                break;
            default:
                break;
        }
        delete msg;
    }
    if (isSrvFound)
    {
        auto d_msg = new Messages::ClientDataMessage(std::vector<GLfloat>{x - GLfloat(0.1), y - GLfloat(0.1), z});
        d_msg->AddressTo = srvaddr;
        agent->sendMessage(d_msg);
    }
}
ClientLogic::~ClientLogic(){
    agent->stop();
}
