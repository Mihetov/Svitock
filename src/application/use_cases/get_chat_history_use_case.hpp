#pragma once

#include <vector>

#include "application/interfaces/message_repository.hpp"

namespace svitock::application::use_cases {

class GetChatHistoryUseCase {
public:
    explicit GetChatHistoryUseCase(const interfaces::MessageRepository& messageRepository) noexcept;

    std::vector<domain::message::Message> execute(const boost::uuids::uuid& chatId, std::size_t limit) const;

private:
    const interfaces::MessageRepository& messageRepository_;
};

} // namespace svitock::application::use_cases
