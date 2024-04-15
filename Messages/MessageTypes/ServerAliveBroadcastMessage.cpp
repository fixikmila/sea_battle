#include "ServerAliveBroadcastMessage.h"

namespace Messages {
    ServerAliveBroadcastMessage::ServerAliveBroadcastMessage(int uid) {
        SrvUid = uid;
        Type = Messages::ServerAliveBroadcast;
    }
} // Messages
