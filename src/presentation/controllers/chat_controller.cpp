#include "presentation/controllers/chat_controller.hpp"

namespace svitock::presentation::controllers {

ChatController::ChatController(application::use_cases::CreateChatUseCase& createChat) noexcept
    : createChat_(createChat) {}

bool ChatController::createDirectChat() const {
    return createChat_.execute(domain::chat::ChatType::direct);
}

bool ChatController::createGroupChat() const {
    return createChat_.execute(domain::chat::ChatType::group);
}

} // namespace svitock::presentation::controllers
