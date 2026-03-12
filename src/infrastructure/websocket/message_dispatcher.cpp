#include "infrastructure/websocket/message_dispatcher.hpp"

#include <boost/uuid/uuid_io.hpp>

#include "infrastructure/websocket/session_manager.hpp"
#include "infrastructure/websocket/websocket_session.hpp"

namespace svitock::infrastructure::websocket {

MessageDispatcher::MessageDispatcher(application::use_cases::SendMessageUseCase& sendMessageUseCase,
                                     SessionManager& sessionManager,
                                     std::shared_ptr<spdlog::logger> logger) noexcept
    : sendMessageUseCase_(sendMessageUseCase), sessionManager_(sessionManager), logger_(std::move(logger)) {}

void MessageDispatcher::dispatchMessage(WebSocketSession& session, const std::string& rawMessage) const {
    nlohmann::json message;
    try {
        message = nlohmann::json::parse(rawMessage);
    } catch (const std::exception& exception) {
        logger_->warn("invalid json: {}", exception.what());
        session.sendText(R"({"type":"error","payload":{"message":"invalid json"}})");
        return;
    }

    const auto type = message.value("type", "");
    if (type == "auth") {
        handleAuth(session, message);
        return;
    }
    if (type == "send_message") {
        handleSendMessage(session, message);
        return;
    }
    if (type == "typing") {
        handleTyping(message);
        return;
    }
    if (type == "message_read") {
        handleReadReceipt(message);
        return;
    }
    if (type == "ping") {
        session.sendText(R"({"type":"pong"})");
        return;
    }

    session.sendText(R"({"type":"error","payload":{"message":"unsupported type"}})");
}

void MessageDispatcher::handleAuth(WebSocketSession& session, const nlohmann::json& message) const {
    const auto userId = message.value("user_id", "");
    if (userId.empty()) {
        session.sendText(R"({"type":"error","payload":{"message":"user_id required"}})");
        return;
    }

    session.setUserId(userId);
    sessionManager_.registerSession(userId, session.shared_from_this());
    logger_->info("authenticated session for user_id={}", userId);
    session.sendText(R"({"type":"auth_ok"})");
}

void MessageDispatcher::handleSendMessage(WebSocketSession& session, const nlohmann::json& message) const {
    const auto chatIdStr = message.value("chat_id", "");
    const auto toUserId = message.value("to_user_id", "");
    const auto payload = message.value("payload", nlohmann::json::object());
    const auto text = payload.value("text", "");

    if (chatIdStr.empty() || text.empty()) {
        session.sendText(R"({"type":"error","payload":{"message":"chat_id and payload.text required"}})");
        return;
    }
    if (session.getUserId().empty()) {
        session.sendText(R"({"type":"error","payload":{"message":"auth required"}})");
        return;
    }

    const auto chatId = boost::uuids::string_generator{}(chatIdStr);
    const auto senderId = boost::uuids::string_generator{}(session.getUserId());

    if (!sendMessageUseCase_.execute(chatId, senderId, text)) {
        session.sendText(R"({"type":"error","payload":{"message":"message not stored"}})");
        return;
    }

    nlohmann::json outgoing{
        {"type", "message_delivered"},
        {"chat_id", chatIdStr},
        {"from_user_id", session.getUserId()},
        {"payload", {"text", text}}};

    session.sendText(outgoing.dump());

    if (!toUserId.empty()) {
        if (const auto recipient = sessionManager_.findSession(toUserId); recipient != nullptr) {
            recipient->sendText(outgoing.dump());
        }
    }

    logger_->info("dispatched message chat_id={} from={}", chatIdStr, session.getUserId());
}

void MessageDispatcher::handleTyping(const nlohmann::json& message) const {
    logger_->debug("typing event: {}", message.dump());
}

void MessageDispatcher::handleReadReceipt(const nlohmann::json& message) const {
    logger_->debug("message_read event: {}", message.dump());
}

} // namespace svitock::infrastructure::websocket
