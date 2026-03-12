#include "infrastructure/dispatcher/message_dispatcher.hpp"

#include <nlohmann/json.hpp>

namespace svitock::infrastructure::dispatcher {

MessageDispatcher::MessageDispatcher(core::actor::ActorSystem& actorSystem,
                                     std::shared_ptr<spdlog::logger> logger) noexcept
    : actorSystem_(actorSystem), logger_(std::move(logger)) {}

void MessageDispatcher::dispatch(const std::string& sessionId, const std::string& rawMessage) const {
    const auto json = nlohmann::json::parse(rawMessage, nullptr, false);
    if (!json.is_object()) {
        logger_->warn("invalid websocket payload");
        return;
    }

    const auto type = json.value("type", "");
    if (type == "auth") {
        auto envelope = std::make_unique<core::actor::ActorEnvelope>();
        envelope->destination = core::actor::ActorType::session;
        envelope->payload = core::actor::AuthEvent{sessionId, json.value("user_id", "")};
        (void)actorSystem_.send(std::move(envelope));
        return;
    }

    if (type == "send_message") {
        auto envelope = std::make_unique<core::actor::ActorEnvelope>();
        envelope->destination = core::actor::ActorType::message;
        const auto payload = json.value("payload", nlohmann::json::object());
        envelope->payload = core::actor::SendMessageEvent{
            sessionId,
            json.value("chat_id", ""),
            json.value("sender_id", ""),
            json.value("recipient_session_id", ""),
            payload.value("text", "")};
        (void)actorSystem_.send(std::move(envelope));
        return;
    }

    if (type == "typing") {
        auto envelope = std::make_unique<core::actor::ActorEnvelope>();
        envelope->destination = core::actor::ActorType::message;
        envelope->payload = core::actor::TypingEvent{json.value("chat_id", ""), json.value("user_id", "")};
        (void)actorSystem_.send(std::move(envelope));
        return;
    }

    if (type == "message_read") {
        auto envelope = std::make_unique<core::actor::ActorEnvelope>();
        envelope->destination = core::actor::ActorType::message;
        envelope->payload = core::actor::ReadEvent{
            json.value("chat_id", ""),
            json.value("user_id", ""),
            json.value("message_id", "")};
        (void)actorSystem_.send(std::move(envelope));
    }
}

} // namespace svitock::infrastructure::dispatcher
