#ifndef SHIPS_ENDMESSAGE_H
#define SHIPS_ENDMESSAGE_H

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include "../Message.h"

namespace Messages {

    class EndMessage : public Message {
    public:
        EndMessage() = default;
        explicit EndMessage(int id);
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

#endif //SHIPS_ENDMESSAGE_H
