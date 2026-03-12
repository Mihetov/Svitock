#pragma once

#include <chrono>
#include <string>

#include <boost/uuid/uuid.hpp>

namespace svitock::domain::message {

class Message {
public:
    Message(boost::uuids::uuid id,
            boost::uuids::uuid chatId,
            boost::uuids::uuid senderId,
            std::string content,
            std::chrono::system_clock::time_point createdAt) noexcept;

    [[nodiscard]] const boost::uuids::uuid& getId() const noexcept;
    [[nodiscard]] const boost::uuids::uuid& getChatId() const noexcept;
    [[nodiscard]] const boost::uuids::uuid& getSenderId() const noexcept;
    [[nodiscard]] const std::string& getContent() const noexcept;
    [[nodiscard]] std::chrono::system_clock::time_point getCreatedAt() const noexcept;

private:
    boost::uuids::uuid id_;
    boost::uuids::uuid chatId_;
    boost::uuids::uuid senderId_;
    std::string content_;
    std::chrono::system_clock::time_point createdAt_;
};

} // namespace svitock::domain::message
