#pragma once

#include <vector>

#include "application/use_cases/get_chat_history_use_case.hpp"
#include "application/use_cases/send_message_use_case.hpp"

namespace svitock::presentation::controllers {

class MessageController {
public:
    MessageController(application::use_cases::SendMessageUseCase& sendMessage,
                      application::use_cases::GetChatHistoryUseCase& getHistory) noexcept;

    bool send(const boost::uuids::uuid& chatId,
              const boost::uuids::uuid& senderId,
              const std::string& content) const;
    std::vector<domain::message::Message> history(const boost::uuids::uuid& chatId, std::size_t limit) const;

private:
    application::use_cases::SendMessageUseCase& sendMessage_;
    application::use_cases::GetChatHistoryUseCase& getHistory_;
};

} // namespace svitock::presentation::controllers
