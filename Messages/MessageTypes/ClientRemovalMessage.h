#ifndef SHIPS_CLIENTREMOVALMESSAGE_H
#define SHIPS_CLIENTREMOVALMESSAGE_H

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include "../Message.h"

namespace Messages {

    class ClientRemovalMessage : public Message{
    public:
        ClientRemovalMessage() = default;
        explicit ClientRemovalMessage(std::pair<std::string, unsigned short> id);
        std::pair<std::string, unsigned short> Id;

    private:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
            ar & boost::serialization::base_object<Message>(*this);
            ar & Id;
        }
    };

} // Messages

#endif //SHIPS_CLIENTREMOVALMESSAGE_H
