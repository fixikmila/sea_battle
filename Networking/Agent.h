#ifndef SHIPS_AGENT_H
#define SHIPS_AGENT_H

#include <boost/thread.hpp>
#include <iostream>
#include <boost/asio/ip/udp.hpp>

#include "../Messages/msg.h"
#include "../Utils/ThreadSafeQueue.h"
#include "../Utils/Serialization.h"


class Agent {
public:
    volatile int PORT = 0;
    volatile bool isWorking = false;
    volatile bool isFailed = false;

    bool start(int ports_range_start, int ports_range_end);
    void stop();

    Messages::Message* getMessage();
    void sendMessage(Messages::Message *msg);

private:
    void RecWorker();
    void TrnWorker();

    boost::thread *recw, *trnw;

    boost::asio::ip::udp::socket* s_;
    ThreadSafeQueue<Messages::Message*> qrec{};
    ThreadSafeQueue<Messages::Message*> qtrn{};
};


#endif //SHIPS_AGENT_H
