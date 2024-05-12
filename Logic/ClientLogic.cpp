//
// Created by milana on 15.04.24.
//

#include "ClientLogic.h"
#include <iostream>
#include "../Gui/ClientGui.h"

ClientLogic::ClientLogic()
{
    agent = new Agent();
    bool s = agent->start(CLIENT_PORT_RANGE_START, CLIENT_PORT_RANGE_END);
    if (!s)
    {
        std::cout << "[ERR] Error while starting networking" << std::endl;
        //return 1;
    }
}
bool ClientLogic::isGood()
{
    return agent->isWorking && !agent->isFailed;
}

void ClientLogic::DispatchMessage(std::map<std::pair<std::string, unsigned short>, std::vector<GLuint>>&data,std::map<std::pair<std::string, unsigned short>, std::vector<GLuint>>&data_cubes, GLuint x, GLuint y, GLuint z,int flag)
{
    Messages::Message* msg;
    msg = agent->getMessage();
    if (msg)
    {
        switch (msg->Type)
        {
            case Messages::Hello:
            {
                in_game = true;
                srvaddr = msg->AddressFrom;
                isSrvFound = true;
                std::cout << "Server was found!" << std::endl;
            }
                break;
            case Messages::ClientDataPropagation:
            {
                auto cd_msg = dynamic_cast<Messages::ClientDataPropagationMessage*>(msg);
                if(cd_msg->Data[3] == 1) {
                   // std::cout<<"flag1"<<std::endl;
                    data.erase(cd_msg->Id);
                    data.insert(std::pair<std::pair<std::string, unsigned short>, std::vector<GLuint>>(cd_msg->Id,
                                                                                                       cd_msg->Data));
                } else{
                    //std::cout<<"flag2"<<std::endl;
                    data_cubes.erase(cd_msg->Id);
                    data_cubes.insert(std::pair<std::pair<std::string, unsigned short>, std::vector<GLuint>>(cd_msg->Id,
                                                                                                       cd_msg->Data));
                }
            }
                break;
            case Messages::ClientRemoval:
            {
                auto cd_msg = dynamic_cast<Messages::ClientRemovalMessage*>(msg);
                data.erase(cd_msg->Id);
                data_cubes.erase(cd_msg->Id);
            }
                break;
            default:
                break;
        }
        delete msg;
    }
    if (isSrvFound)
    {
        if(flag == 1)return;
        auto d_msg = new Messages::ClientDataMessage(std::vector<GLuint>{x, y, z, static_cast<unsigned int>(flag)});
        d_msg->AddressTo = srvaddr;
        agent->sendMessage(d_msg);
    }
    else if(ClientGui::F == 3)
    {
        for (int i = SRV_PORT_RANGE_START; i <= SRV_PORT_RANGE_END; i++)
        {
            auto hmsg = new Messages::HelloMessage(i);
            hmsg->AddressTo = std::pair<std::string, unsigned short>(SRV_ADDR, i);
            agent->sendMessage(hmsg);
        }
    }
}
ClientLogic::~ClientLogic(){
    agent->stop();
}
