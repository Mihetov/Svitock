#pragma once

#include "application/use_cases/create_chat_use_case.hpp"

namespace svitock::presentation::controllers {

class ChatController {
public:
    explicit ChatController(application::use_cases::CreateChatUseCase& createChat) noexcept;

    bool createDirectChat() const;
    bool createGroupChat() const;

private:
    application::use_cases::CreateChatUseCase& createChat_;
};

} // namespace svitock::presentation::controllers
