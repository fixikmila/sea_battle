#ifndef SHIPS_MESSAGE_H
#define SHIPS_MESSAGE_H

#include <string>
#include <utility>
#include <boost/serialization/access.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/export.hpp>

namespace Messages {

    enum MsgType {
        None = 0,
        ServerAliveBroadcast = 1,
        ClientData = 2,
        ClientDataPropagation = 3,
        ClientRemoval = 4,
        Hello = 5
    };

    class Message {
    public:
        MsgType Type = MsgType::None;
        std::pair<std::string, unsigned short> AddressFrom, AddressTo;
        int Id;

        virtual ~Message() = default;
    private:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
            ar & Type;
            ar & AddressFrom;
            ar & AddressTo;
            ar & Id;
        }

        template<class Archive>
        void serialize(Archive & ar, std::pair<std::string, unsigned short> & p, const unsigned int version) {
            ar & p.first;
            ar & p.second;
        }


    };
}

#endif //SHIPS_MESSAGE_H
