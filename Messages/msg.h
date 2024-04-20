#ifndef SHIPS_MSG_H
#define SHIPS_MSG_H

#include <boost/serialization/access.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

#include "Message.h"
#include "MessageTypes/ClientDataMessage.h"
#include "MessageTypes/ClientDataPropagationMessage.h"
#include "MessageTypes/ServerAliveBroadcastMessage.h"
#include "MessageTypes/ClientRemovalMessage.h"
#include "MessageTypes/HelloMessage.h"

#endif //SHIPS_MSG_H
