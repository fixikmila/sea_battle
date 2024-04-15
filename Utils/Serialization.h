#ifndef SHIPS_SERIALIZATION_H
#define SHIPS_SERIALIZATION_H

#include <string>
#include <boost/archive/binary_oarchive.hpp>
#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include "../Messages/msg.h"


template <typename T>
std::string Serialize(const T* msg) {
    std::ostringstream oss;
    {
        boost::archive::binary_oarchive oa(oss);
        oa << msg;
    }
    return oss.str();
}

template <typename T>
T* Deserialize(const std::string& serialized_data) {
    std::istringstream iss(serialized_data);
    T* deserialized_msg = nullptr;
    {
        boost::archive::binary_iarchive ia(iss);
        ia >> deserialized_msg;
    }
    return deserialized_msg;
}

#endif //SHIPS_SERIALIZATION_H
