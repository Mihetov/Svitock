#include "presentation/controllers/message_controller.hpp"

namespace svitock::presentation::controllers {

MessageController::MessageController(application::use_cases::SendMessageUseCase& sendMessage,
                                     application::use_cases::GetChatHistoryUseCase& getHistory) noexcept
    : sendMessage_(sendMessage), getHistory_(getHistory) {}

bool MessageController::send(const boost::uuids::uuid& chatId,
                             const boost::uuids::uuid& senderId,
                             const std::string& content) const {
    return sendMessage_.execute(chatId, senderId, content);
}

std::vector<domain::message::Message> MessageController::history(const boost::uuids::uuid& chatId,
                                                                 const std::size_t limit) const {
    return getHistory_.execute(chatId, limit);
}

} // namespace svitock::presentation::controllers
