#pragma once

#include <string>

#include "application/interfaces/message_repository.hpp"

namespace svitock::application::use_cases {

class SendMessageUseCase {
public:
    explicit SendMessageUseCase(interfaces::MessageRepository& messageRepository) noexcept;

    bool execute(const boost::uuids::uuid& chatId,
                 const boost::uuids::uuid& senderId,
                 const std::string& content) const;

private:
    interfaces::MessageRepository& messageRepository_;
};

} // namespace svitock::application::use_cases
