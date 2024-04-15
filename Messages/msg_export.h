#ifndef SHIPS_MSG_EXPORT_H
#define SHIPS_MSG_EXPORT_H

#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/export.hpp>
#include "msg.h"

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Messages::Message)
BOOST_CLASS_EXPORT(Messages::Message)
BOOST_CLASS_EXPORT(Messages::ServerAliveBroadcastMessage)
BOOST_CLASS_EXPORT(Messages::ClientDataMessage)
BOOST_CLASS_EXPORT(Messages::ClientDataPropagationMessage)
BOOST_CLASS_EXPORT(Messages::ClientRemovalMessage)

#endif //SHIPS_MSG_EXPORT_H
