#pragma once

#include <string>

#include <boost/uuid/uuid.hpp>

namespace svitock::application::services {

class RealtimeDispatcher {
public:
    virtual ~RealtimeDispatcher() = default;

    virtual void publishToChat(const boost::uuids::uuid& chatId, const std::string& payload) = 0;
};

} // namespace svitock::application::services
