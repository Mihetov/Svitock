#include "infrastructure/actors/chat_actor.hpp"

#include <nlohmann/json.hpp>

#include "core/actor/actor_system.hpp"

namespace svitock::infrastructure::actors {

ChatActor::ChatActor(std::shared_ptr<core::actor::Mailbox> mailbox,
                     core::actor::ActorSystem& actorSystem,
                     std::shared_ptr<spdlog::logger> logger)
    : Actor(std::move(mailbox)), actorSystem_(actorSystem), logger_(std::move(logger)) {}

void ChatActor::handle(core::actor::ActorEnvelope&& envelope) {
    if (std::holds_alternative<core::actor::SendMessageEvent>(envelope.payload)) {
        const auto& event = std::get<core::actor::SendMessageEvent>(envelope.payload);

        auto persist = std::make_unique<core::actor::ActorEnvelope>();
        persist->destination = core::actor::ActorType::storage;
        persist->payload = core::actor::StoragePersistMessage{event.chatId, event.senderId, event.text};
        (void)actorSystem_.send(std::move(persist));

        auto echo = std::make_unique<core::actor::ActorEnvelope>();
        echo->destination = core::actor::ActorType::session;
        echo->payload = core::actor::SessionOutgoingEvent{
            event.sessionId,
            nlohmann::json{{"type", "message_delivered"},
                           {"chat_id", event.chatId},
                           {"payload", {{"text", event.text}}}}};
        (void)actorSystem_.send(std::move(echo));

        if (!event.recipientId.empty()) {
            auto recipient = std::make_unique<core::actor::ActorEnvelope>();
            recipient->destination = core::actor::ActorType::session;
            recipient->payload = core::actor::SessionOutgoingEvent{
                event.recipientId,
                nlohmann::json{{"type", "incoming_message"},
                               {"chat_id", event.chatId},
                               {"payload", {{"text", event.text}, {"from", event.senderId}}}}};
            (void)actorSystem_.send(std::move(recipient));
        }

        logger_->debug("chat_actor routed message chat_id={}", event.chatId);
    }
}

} // namespace svitock::infrastructure::actors
