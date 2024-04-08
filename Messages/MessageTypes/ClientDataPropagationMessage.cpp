#include "ClientDataPropagationMessage.h"

#include <utility>

namespace Messages {
    ClientDataPropagationMessage::ClientDataPropagationMessage(std::pair<std::string, unsigned short> id, std::vector<GLfloat> data) {
        Type = Messages::MsgType::ClientDataPropagation;
        Id = std::move(id);
        ClientDataPropagationMessage::UpdateData(std::move(data));
    }

    void ClientDataPropagationMessage::UpdateData(std::vector<GLfloat> data) {
        Data = std::move(data);
    }
} // Messages