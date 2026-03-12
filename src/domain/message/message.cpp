#include "domain/message/message.hpp"

namespace svitock::domain::message {

Message::Message(boost::uuids::uuid id,
                 boost::uuids::uuid chatId,
                 boost::uuids::uuid senderId,
                 std::string content,
                 std::chrono::system_clock::time_point createdAt) noexcept
    : id_(id), chatId_(chatId), senderId_(senderId), content_(std::move(content)), createdAt_(createdAt) {}

const boost::uuids::uuid& Message::getId() const noexcept { return id_; }

const boost::uuids::uuid& Message::getChatId() const noexcept { return chatId_; }

const boost::uuids::uuid& Message::getSenderId() const noexcept { return senderId_; }

const std::string& Message::getContent() const noexcept { return content_; }

std::chrono::system_clock::time_point Message::getCreatedAt() const noexcept { return createdAt_; }

} // namespace svitock::domain::message
