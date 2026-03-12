#pragma once

#include "application/interfaces/chat_repository.hpp"

namespace svitock::application::use_cases {

class CreateChatUseCase {
public:
    explicit CreateChatUseCase(interfaces::ChatRepository& chatRepository) noexcept;

    bool execute(domain::chat::ChatType type) const;

private:
    interfaces::ChatRepository& chatRepository_;
};

} // namespace svitock::application::use_cases
