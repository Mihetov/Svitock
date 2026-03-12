#pragma once

#include <optional>

#include "domain/chat/chat.hpp"

namespace svitock::application::interfaces {

class ChatRepository {
public:
    virtual ~ChatRepository() = default;

    virtual bool create(const domain::chat::Chat& chat) = 0;
    virtual std::optional<domain::chat::Chat> findById(const boost::uuids::uuid& id) const = 0;
};

} // namespace svitock::application::interfaces
