#include "application/use_cases/get_chat_history_use_case.hpp"

namespace svitock::application::use_cases {

GetChatHistoryUseCase::GetChatHistoryUseCase(const interfaces::MessageRepository& messageRepository) noexcept
    : messageRepository_(messageRepository) {}

std::vector<domain::message::Message> GetChatHistoryUseCase::execute(const boost::uuids::uuid& chatId,
                                                                     const std::size_t limit) const {
    return messageRepository_.getByChat(chatId, limit);
}

} // namespace svitock::application::use_cases
