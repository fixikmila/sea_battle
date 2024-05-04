#include "ClientDataMessage.h"

#include <utility>

namespace Messages {

    ClientDataMessage::ClientDataMessage(std::vector<GLuint> position) {
        Type = Messages::MsgType::ClientData;
        ClientDataMessage::UpdatePosition(std::move(position));
    }

    void ClientDataMessage::UpdatePosition(std::vector<GLuint> position) {
        Position = std::move(position);
    }
} // Messages
