#include <sstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <memory>
#include <iostream>
#include "../Messages/msg.h"

int main() {
    // Create a ServerAliveBroadcastMessage object
    auto* msg = new Messages::ServerAliveBroadcastMessage(45);

    // Cast the ServerAliveBroadcastMessage object to a Message object
    Messages::Message* base_msg = msg;

    // Serialize the Message object
    std::ostringstream oss;
    {
        boost::archive::binary_oarchive oa(oss);
        oa << base_msg;
    }
    std::string serialized_data = oss.str();

    // Deserialize the Message object
    std::istringstream iss(serialized_data);
    Messages::Message* deserialized_msg = nullptr;
    {
        boost::archive::binary_iarchive ia(iss);
        ia >> deserialized_msg;
    }

    // Check the deserialized object
    if (auto* r = dynamic_cast<Messages::ServerAliveBroadcastMessage*>(deserialized_msg)) {
        std::cout << "SrvUid: " << r->SrvUid << std::endl;
    } else {
        std::cout << "Failed to cast to ServerAliveBroadcastMessage" << std::endl;
    }

    // Clean up the deserialized object
    delete deserialized_msg;
    delete msg;

    return 0;
}
