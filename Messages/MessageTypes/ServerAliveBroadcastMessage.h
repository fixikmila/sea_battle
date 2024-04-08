#ifndef SHIPS_SERVERALIVEBROADCASTMESSAGE_H
#define SHIPS_SERVERALIVEBROADCASTMESSAGE_H

#include <boost/serialization/base_object.hpp>
#include "../Message.h"

namespace Messages {

    class ServerAliveBroadcastMessage : public Message{
    public:
        ServerAliveBroadcastMessage() = default;
        explicit ServerAliveBroadcastMessage(int uid);
        int SrvUid = 0;


    private:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
            ar & boost::serialization::base_object<Message>(*this);
            ar & SrvUid;
        }


    };

} // Messages

#endif //SHIPS_SERVERALIVEBROADCASTMESSAGE_H
