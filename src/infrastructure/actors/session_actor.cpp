#include "infrastructure/actors/session_actor.hpp"

#include <mutex>
#include <shared_mutex>

#include "core/actor/actor_system.hpp"
#include "infrastructure/network/websocket/websocket_session.hpp"

namespace svitock::infrastructure::actors {

SessionActor::SessionActor(std::shared_ptr<core::actor::Mailbox> mailbox,
                           core::actor::ActorSystem& actorSystem,
                           std::shared_ptr<spdlog::logger> logger)
    : Actor(std::move(mailbox)), actorSystem_(actorSystem), logger_(std::move(logger)) {}

void SessionActor::handle(core::actor::ActorEnvelope&& envelope) {
    if (std::holds_alternative<core::actor::SessionOutgoingEvent>(envelope.payload)) {
        const auto& outgoing = std::get<core::actor::SessionOutgoingEvent>(envelope.payload);
        std::shared_lock lock(mutex_);
        const auto it = transports_.find(outgoing.sessionId);
        if (it == transports_.end()) {
            return;
        }
        if (auto transport = it->second.lock(); transport != nullptr) {
            transport->send(outgoing.payload.dump());
        }
        return;
    }

    if (std::holds_alternative<core::actor::AuthEvent>(envelope.payload)) {
        const auto& auth = std::get<core::actor::AuthEvent>(envelope.payload);
        logger_->info("session authenticated session_id={} user_id={}", auth.sessionId, auth.userId);
    }
}

void SessionActor::bindTransport(const std::string& sessionId,
                                 std::weak_ptr<network::websocket::SessionTransport> transport) {
    std::unique_lock lock(mutex_);
    transports_[sessionId] = std::move(transport);
}

void SessionActor::unbindTransport(const std::string& sessionId) {
    std::unique_lock lock(mutex_);
    transports_.erase(sessionId);
}

} // namespace svitock::infrastructure::actors
