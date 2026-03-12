#include "domain/chat/chat.hpp"

namespace svitock::domain::chat {

Chat::Chat(boost::uuids::uuid id,
           ChatType type,
           std::chrono::system_clock::time_point createdAt) noexcept
    : id_(id), type_(type), createdAt_(createdAt) {}

const boost::uuids::uuid& Chat::getId() const noexcept { return id_; }

ChatType Chat::getType() const noexcept { return type_; }

std::chrono::system_clock::time_point Chat::getCreatedAt() const noexcept { return createdAt_; }

} // namespace svitock::domain::chat
