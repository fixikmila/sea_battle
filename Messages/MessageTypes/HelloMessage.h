#ifndef SHIPS_HELLOMESSAGE_H
#define SHIPS_HELLOMESSAGE_H

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include "../Message.h"

namespace Messages {

    class HelloMessage : public Message {
    public:
        HelloMessage() = default;
        explicit HelloMessage(int id);
        int Id;
    private:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
            ar & boost::serialization::base_object<Message>(*this);
            ar & Id;
        }
    };

}

#endif //SHIPS_HELLOMESSAGE_H
