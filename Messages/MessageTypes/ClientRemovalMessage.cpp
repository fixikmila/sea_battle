#include "ClientRemovalMessage.h"

#include <utility>

namespace Messages {
    ClientRemovalMessage::ClientRemovalMessage(std::pair<std::string, unsigned short> id)
    {
        Type = Messages::MsgType::ClientRemoval;
        Id = std::move(id);
    };
} // Messages