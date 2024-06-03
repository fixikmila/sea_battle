//
// Created by milana on 15.04.24.
//
#include <bits/stdc++.h>
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
bool deleted[6][6][6];
std::vector<int> canIdel()
{
    std::vector<int> a;
    for(int i=0;i<6;i++)
    {
        for(int j=0;j<6;j++)
        {
            for(int q=0;q<6;q++)
            {
                if(!deleted[i][j][q]) {
                    int w = ClientGui::type[i][j][q];
                    int x = ClientGui::move[w][0], y = ClientGui::move[w][1], z = ClientGui::move[w][2];
                    int x1 = i + x, y1 = j + y, z1 = z + q;
                    int kot = 0;
                    while (x1 >= 0 && y1 >= 0 && z1 >= 0 && x1 < 6 && y1 < 6 && z1 < 6) {
                        if (!deleted[x1][y1][z1]) {
                            kot = 1;
                        }
                        x1 += x;
                        y1 += y;
                        z1 += z;
                    }
                    if (kot == 0) {
                        a = {i, j, q};
                        return a;
                    }
                }
            }
        }
    }
    return a;
}

int skolko()
{
    std::vector<int> a;
    int ans=0;
    while (true)
    {
        //std::cout<<"a.size"<<a.size()<<std::endl;
        a=canIdel();
        if(a.size()==0)
        {
            break;
        }
        else
        {
            deleted[a[0]][a[1]][a[2]] = true;
        }
        ans++;
    }
    return ans;

}
void ClientLogic::DispatchMessage(std::map<std::pair<std::string, unsigned short>, std::vector<GLuint>>&data,std::map<std::pair<std::string, unsigned short>, std::vector<GLuint>>&data_cubes, GLuint x, GLuint y, GLuint z,int flag)
{
    Messages::Message* msg;
    msg = agent->getMessage();
    //std::cout<<"dispatch "<<msg<<std::endl;
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
                std::mt19937 rng(rand_num);
                for(int i = 0; i < 6; i++){
                    for(int j = 0; j < 6; j++){
                        for(int k = 0; k < 6; k++){
                            ClientGui::type[i][j][k] = rng() % 6;
                        }
                    }
                }
                how_many = skolko();
                std::cout<<"how_many: "<<how_many<<std::endl;
            }
                break;
            case Messages::ClientDataPropagation:
            {
                auto cd_msg = dynamic_cast<Messages::ClientDataPropagationMessage*>(msg);
                if(cd_msg->Data[3] == 4){
                    /*int ii = cd_msg->Data[0];
                    int jj = cd_msg->Data[1];
                    int kk = cd_msg->Data[2];
                    int val = cd_msg->Data[4];
                    ClientGui::type[ii][jj][kk] = val;*/
                    rand_num = cd_msg->Data[0];
                    //std::cout<<ii<<" "<<jj<<" "<<kk<<" "<<val<<std::endl;
                }
                else if(cd_msg->Data[3] == 1) {
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
    if(game_end){
        game_end = false;
        if(isSrvFound) {
            auto hmsg = new Messages::EndMessage(1);
            hmsg->AddressTo = srvaddr;
            agent->sendMessage(hmsg);
        } else{
            for (int i = SRV_PORT_RANGE_START; i <= SRV_PORT_RANGE_END; i++)
            {
                auto hmsg = new Messages::EndMessage(1);
                hmsg->AddressTo = std::pair<std::string, unsigned short>(SRV_ADDR, i);
                agent->sendMessage(hmsg);
            }
        }
        isSrvFound = false;
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
