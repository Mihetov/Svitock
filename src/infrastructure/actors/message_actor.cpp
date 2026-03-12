#include "infrastructure/actors/message_actor.hpp"

#include "core/actor/actor_system.hpp"

namespace svitock::infrastructure::actors {

MessageActor::MessageActor(std::shared_ptr<core::actor::Mailbox> mailbox,
                           core::actor::ActorSystem& actorSystem,
                           std::shared_ptr<spdlog::logger> logger)
    : Actor(std::move(mailbox)), actorSystem_(actorSystem), logger_(std::move(logger)) {}

void MessageActor::handle(core::actor::ActorEnvelope&& envelope) {
    if (std::holds_alternative<core::actor::SendMessageEvent>(envelope.payload)) {
        auto forward = std::make_unique<core::actor::ActorEnvelope>();
        forward->destination = core::actor::ActorType::chat;
        forward->payload = std::get<core::actor::SendMessageEvent>(envelope.payload);
        (void)actorSystem_.send(std::move(forward));
    }
    if (std::holds_alternative<core::actor::TypingEvent>(envelope.payload)) {
        const auto& typing = std::get<core::actor::TypingEvent>(envelope.payload);
        logger_->debug("typing chat_id={} user_id={}", typing.chatId, typing.userId);
    }
}

} // namespace svitock::infrastructure::actors
