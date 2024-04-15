#include "Agent.h"

void Agent::RecWorker()
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
            msg->AddressFrom = std::pair<std::string, unsigned short>(sender_endpoint.address().to_string(),sender_endpoint.port());
            qrec.push(msg);
        }
    }
}

void Agent::TrnWorker()
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
            auto rendp = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(msg->AddressTo.first), msg->AddressTo.second);
            s_->send_to(boost::asio::buffer(message), rendp);

        }

    }
}

bool Agent::start(int ports_range_start, int ports_range_end)
{
    if (ports_range_end < ports_range_start)
        return false;
    std::cout << "[LOG] Starting agent" << std::endl;
    boost::asio::io_context io_context;
    PORT = 0;
    for (int i = ports_range_start; i <= ports_range_end; i++)
    {
        try
        {
            boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address_v4::any(), i);
            s_ = new boost::asio::ip::udp::socket(io_context, endpoint);
            s_->set_option(boost::asio::ip::udp::socket::broadcast(true));
            //TODO: remove this option at next setup

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
        return false;
    }
    std::cout << "[LOG] Agent acquired port " << s_->local_endpoint().port() << std::endl;
    isWorking = true;

    recw = new boost::thread([this] { RecWorker(); });
    trnw = new boost::thread([this] { TrnWorker(); });
    std::cout << "[LOG] Receive/Transmit workers started" << std::endl;

    return true;
}

void Agent::stop()
{
    isWorking = false;
    recw->join();
    trnw->join();
    delete recw;
    delete trnw;
    s_->close();
}

Messages::Message* Agent::getMessage()
{
    Messages::Message* msg = nullptr;
    if (qrec.try_pop(msg))
    {
        return msg;
    }
    else
    {
        return nullptr;
    }
}


void Agent::sendMessage(Messages::Message *msg)
{
    qtrn.push(msg);
}
