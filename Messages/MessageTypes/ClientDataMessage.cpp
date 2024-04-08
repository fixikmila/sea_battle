#include "ClientDataMessage.h"

#include <utility>

namespace Messages {

    ClientDataMessage::ClientDataMessage(std::vector<GLfloat> position) {
        Type = Messages::MsgType::ClientData;
        ClientDataMessage::UpdatePosition(std::move(position));
    }

    void ClientDataMessage::UpdatePosition(std::vector<GLfloat> position) {
        Position = std::move(position);
    }
} // Messages