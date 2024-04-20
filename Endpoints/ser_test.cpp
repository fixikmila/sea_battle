#include <sstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <iostream>
#include "../Messages/msg_export.h"

bool HelloTest()
{
    bool f = true;
    auto* msg = new Messages::HelloMessage(42);
    Messages::Message* base_msg = msg;
    std::ostringstream oss;
    {
        boost::archive::binary_oarchive oa(oss);
        oa << base_msg;
    }
    std::string serialized_data = oss.str();
    std::istringstream iss(serialized_data);
    Messages::Message* deserialized_msg = nullptr;
    {
        boost::archive::binary_iarchive ia(iss);
        ia >> deserialized_msg;
    }
    if (auto* r = dynamic_cast<Messages::HelloMessage*>(deserialized_msg)) {
        std::cout << "Hello id: " << r->Id << std::endl;
    } else {
        std::cout << "HelloMessage test failed" << std::endl;
        f = false;
    }
    delete deserialized_msg;
    delete msg;
    return f;
}
bool ClientDataTest()
{
    bool f = true;
    auto* msg = new Messages::ClientDataMessage(std::vector<GLfloat>{27});
    Messages::Message* base_msg = msg;
    std::ostringstream oss;
    {
        boost::archive::binary_oarchive oa(oss);
        oa << base_msg;
    }
    std::string serialized_data = oss.str();
    std::istringstream iss(serialized_data);
    Messages::Message* deserialized_msg = nullptr;
    {
        boost::archive::binary_iarchive ia(iss);
        ia >> deserialized_msg;
    }
    if (auto* r = dynamic_cast<Messages::ClientDataMessage*>(deserialized_msg)) {
        std::cout << "Data value: " << r->Position[0] << std::endl;
    } else {
        std::cout << "ClientDataMessage test failed" << std::endl;
        f = false;
    }
    delete deserialized_msg;
    delete msg;
    return f;
}

int main() {

    HelloTest();
    ClientDataTest();

    return 0;
}
