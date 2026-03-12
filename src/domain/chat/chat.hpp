#pragma once

#include <chrono>
#include <cstdint>

#include <boost/uuid/uuid.hpp>

namespace svitock::domain::chat {

enum class ChatType : std::uint8_t {
    direct = 0,
    group = 1,
};

class Chat {
public:
    Chat(boost::uuids::uuid id,
         ChatType type,
         std::chrono::system_clock::time_point createdAt) noexcept;

    [[nodiscard]] const boost::uuids::uuid& getId() const noexcept;
    [[nodiscard]] ChatType getType() const noexcept;
    [[nodiscard]] std::chrono::system_clock::time_point getCreatedAt() const noexcept;

private:
    boost::uuids::uuid id_;
    ChatType type_;
    std::chrono::system_clock::time_point createdAt_;
};

} // namespace svitock::domain::chat
