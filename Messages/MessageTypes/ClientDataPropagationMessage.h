#ifndef SHIPS_CLIENTDATAPROPAGATIONMESSAGE_H
#define SHIPS_CLIENTDATAPROPAGATIONMESSAGE_H

#include <vector>
#include <GL/glew.h>
#include <boost/serialization/base_object.hpp>
#include "../Message.h"

namespace Messages {

    class ClientDataPropagationMessage : public Message {
    public:
        ClientDataPropagationMessage() = default;
        explicit ClientDataPropagationMessage(std::pair<std::string, unsigned short> id, std::vector<GLfloat> data);

        void UpdateData(std::vector<GLfloat> data);
        std::vector<GLfloat> Data = {};
        std::pair<std::string, unsigned short> Id;


    private:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
            ar & boost::serialization::base_object<Message>(*this);
            ar & Data;
            ar & Id;
        }
    };

} // Messages

#endif //SHIPS_CLIENTDATAPROPAGATIONMESSAGE_H
