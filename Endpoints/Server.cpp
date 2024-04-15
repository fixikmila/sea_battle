#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>
#include <boost/asio.hpp>

#include "../Messages/msg_export.h"
#include "../Utils/Config.h"
#include "../Utils/ThreadSafeQueue.h"
#include "../Utils/Serialization.h"
#include "../Logic/ServerLogic.h"

volatile bool isWorking;
volatile bool isFailed;
volatile int PORT;

ThreadSafeQueue<Messages::Message*> qrec{};
ThreadSafeQueue<Messages::Message*> qtrn{};

boost::asio::ip::udp::socket* s_;


void RecWorker()
{
    while (isWorking)
    {
        std::string recvbuf(1024, '\0');
        boost::asio::ip::udp::endpoint sender_endpoint;
        size_t bytes_recv = s_->receive_from(boost::asio::buffer(recvbuf), sender_endpoint);
        recvbuf.resize(bytes_recv);
        auto msg = Deserialize<Messages::Message>(recvbuf);
        if (msg)
        {
            msg->AddressFrom = std::pair<std::string, unsigned short>(sender_endpoint.address().to_string(), sender_endpoint.port());
            qrec.push(msg);
        }
    }
}

void TrnWorker()
{
    Messages::Message* msg;
    bool isPopped;
    auto lendp = s_->local_endpoint();

    while (isWorking)
    {
        isPopped = qtrn.try_pop(msg);
        if (isPopped)
        {
            msg->AddressFrom = std::pair<std::string, unsigned short>(lendp.address().to_string(), lendp.port());
            auto message = Serialize<Messages::Message>(msg);
            if (msg->AddressTo.second == 0)
            {
                for (int i = CLIENT_PORT_RANGE_START; i <= CLIENT_PORT_RANGE_END; i++)
                {
                    boost::asio::ip::udp::endpoint broadcast_endpoint(boost::asio::ip::address_v4::broadcast(), i);
                    s_->send_to(boost::asio::buffer(message), broadcast_endpoint);
                }
            }
            else
            {
                auto rendp = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(msg->AddressTo.first), msg->AddressTo.second);
                s_->send_to(boost::asio::buffer(message), rendp);
            }
        }
    }
}

void BroadcastWorker()
{
    auto msg = new Messages::ServerAliveBroadcastMessage(SRV_UID);
    auto adr = boost::asio::ip::address_v4::broadcast();

    msg->AddressTo = std::pair<std::string, unsigned short>(std::string(adr.to_string()), 0);
    while (isWorking)
    {
        qtrn.push(msg);
        boost::this_thread::sleep_for(boost::chrono::milliseconds(SRV_BROADCAST_INTERVAL));
    }
}
int main()
{
    // INIT
    isFailed = false;
    isWorking = true;
    std::cout << "[LOG] Starting server" << std::endl;

    boost::asio::io_context io_context;
    PORT = 0;
    for (int i = SRV_PORT_RANGE_START; i <= SRV_PORT_RANGE_END; i++)
    {
        try
        {
            boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address_v4::any(), i);
            s_ = new boost::asio::ip::udp::socket(io_context, endpoint);
            s_->set_option(boost::asio::ip::udp::socket::broadcast(true));
            PORT = i;
            break;
        }
        catch (const boost::system::system_error& e)
        {
            continue;
        }
    }
    if (PORT == 0)
    {
        std::cout << "[ERR] Error while binding socket" << std::endl;
        return 1;
    }
    std::cout << "[LOG] Server acquired port " << PORT << std::endl;

    boost::thread recw(RecWorker);
    boost::thread trnw(TrnWorker);
    std::cout << "[LOG] Receive/Transmit workers started" << std::endl;
    boost::thread brdw(BroadcastWorker);
    std::cout << "[LOG] Broadcast worker started" << std::endl;


    // MAIN PART HERE
    std::cout << "[LOG] Starting message dispatch loop.." << std::endl;
    Messages::Message* msg;
    bool isPopped;

    std::map<std::pair<std::string, unsigned short>, boost::chrono::system_clock::time_point> timestamps = {};

    while (isWorking && !isFailed)
    {
        isPopped = qrec.try_pop(msg);
        if (isPopped)
        {
            switch (msg->Type)
            {
                case Messages::ClientData:
                    {
                        timestamps.erase(msg->AddressFrom);
                        timestamps.insert(std::pair<std::pair<std::string, unsigned short>, boost::chrono::system_clock::time_point>(msg->AddressFrom, boost::chrono::system_clock::now()));
                        auto cd_msg = dynamic_cast<Messages::ClientDataMessage*>(msg);
                        for (const auto& [key, value] : timestamps)
                        {
                            if (key != msg->AddressFrom)
                            {
                                auto prop = new Messages::ClientDataPropagationMessage(cd_msg->AddressFrom, cd_msg->Position);
                                prop->AddressTo = key;
                                qtrn.push(prop);
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
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
                    qtrn.push(del_msg);
                }
            }
        }
    }

    // OUTRO
    brdw.join();
    recw.join();
    trnw.join();
    s_->close();
    std::cout << "[LOG] exiting" << std::endl;
    return 0;
}