#ifndef SHIPS_CLIENTDATAMESSAGE_H
#define SHIPS_CLIENTDATAMESSAGE_H

#include <GL/glew.h>
#include <vector>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include "../Message.h"

namespace Messages {

    class ClientDataMessage : public Message {
    public:
        ClientDataMessage() = default;
        explicit ClientDataMessage(std::vector<GLuint> position);
        void UpdatePosition(std::vector<GLuint> position);
        std::vector<GLuint> Position = {0, 0, 0};

    private:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
            ar & boost::serialization::base_object<Message>(*this);
            ar & Position;
        }

    };

} // Messages

#endif //SHIPS_CLIENTDATAMESSAGE_H
