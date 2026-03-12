#pragma once

#include <memory>
#include <string>

#include <boost/uuid/string_generator.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/logger.h>

#include "application/use_cases/send_message_use_case.hpp"

namespace svitock::infrastructure::websocket {

class SessionManager;
class WebSocketSession;

class MessageDispatcher {
public:
    MessageDispatcher(application::use_cases::SendMessageUseCase& sendMessageUseCase,
                      SessionManager& sessionManager,
                      std::shared_ptr<spdlog::logger> logger) noexcept;

    void dispatchMessage(WebSocketSession& session, const std::string& rawMessage) const;

private:
    void handleAuth(WebSocketSession& session, const nlohmann::json& message) const;
    void handleSendMessage(WebSocketSession& session, const nlohmann::json& message) const;
    void handleTyping(const nlohmann::json& message) const;
    void handleReadReceipt(const nlohmann::json& message) const;

    application::use_cases::SendMessageUseCase& sendMessageUseCase_;
    SessionManager& sessionManager_;
    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace svitock::infrastructure::websocket
