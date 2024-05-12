#include "ClientGameEnding.h"

namespace Messages {

    Messages::EndMessage::EndMessage(int id) {
        Id = id;
        Type = Messages::End;
    }

}