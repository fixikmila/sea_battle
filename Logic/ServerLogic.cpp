//
// Created by milana on 15.04.24.
//

#include "ServerLogic.h"

ServerLogic::ServerLogic() {
    agent = new Agent;
    bool s = agent->start(SRV_PORT_RANGE_START, SRV_PORT_RANGE_START);
    if (!s)
    {
        std::cout << "[ERR] Error while starting networking" << std::endl;
    }
}

ServerLogic::~ServerLogic()
{
    agent->stop();
    delete agent;
}

bool ServerLogic::isGood()
{
    return agent->isWorking && !agent->isFailed;
}

void ServerLogic::DispatchMessage(std::map<std::pair<std::string, unsigned short>, boost::chrono::system_clock::time_point>& timestamps)
{
    auto* msg = agent->getMessage();
    if (msg)
    {
        bool isValid = true;
        switch (msg->Type)
        {
            case Messages::Hello:
                {
                    auto cd_msg = dynamic_cast<Messages::HelloMessage*>(msg);
                    auto answ = new Messages::HelloMessage(cd_msg->Id);
                    answ->AddressTo = cd_msg->AddressFrom;
                    agent->sendMessage(answ);
                    std::cout << "Client was found!" << std::endl;
                }
                break;
            case Messages::ClientData:
                {
                    auto cd_msg = dynamic_cast<Messages::ClientDataMessage*>(msg);
                    for (const auto& [key, value] : timestamps)
                    {
                        if (key != msg->AddressFrom)
                        {
                            auto prop = new Messages::ClientDataPropagationMessage(cd_msg->AddressFrom, cd_msg->Position);
                            prop->AddressTo = key;
                            agent->sendMessage(prop);
                        }
                    }

                }
                break;
            default:
                isValid = false;
                break;
        }
        if (isValid)
        {
            timestamps.erase(msg->AddressFrom);
            timestamps.insert(std::pair<std::pair<std::string, unsigned short>, boost::chrono::system_clock::time_point>(msg->AddressFrom, boost::chrono::system_clock::now()));
        }
        delete msg;
    }
    // cleanup
    auto ts = boost::chrono::system_clock::now();
    std::vector<std::pair<std::string, unsigned short>> keysToErase;
    for (const auto& [key, value] : timestamps)
    {
        auto duration = boost::chrono::duration_cast<boost::chrono::milliseconds>(ts - value);
        if (duration.count() > CLIENT_TIMEOUT)
        {
            keysToErase.emplace_back(key);
        }
    }
    for (const auto& key : keysToErase)
    {
        timestamps.erase(key);
    }

    // send cleanup messages
    if (!keysToErase.empty())
    {
        for (const auto &[key, value]: timestamps) {
            for (const auto &deladdr: keysToErase) {
                auto del_msg = new Messages::ClientRemovalMessage(deladdr);
                del_msg->AddressTo = key;
                agent->sendMessage(del_msg);
            }
        }
    }
}
