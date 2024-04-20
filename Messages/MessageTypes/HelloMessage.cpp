#include "HelloMessage.h"

namespace Messages {

    Messages::HelloMessage::HelloMessage(int id) {
        Id = id;
        Type = Messages::Hello;
    }

}