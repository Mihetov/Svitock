#pragma once

#include <memory>
#include <shared_mutex>
#include <string>
#include <unordered_map>

#include <spdlog/logger.h>

#include "core/actor/actor.hpp"

namespace svitock::core::actor {
class ActorSystem;
}

namespace svitock::infrastructure::network::websocket {
class SessionTransport;
}

namespace svitock::infrastructure::actors {

class SessionActor final : public core::actor::Actor {
public:
    SessionActor(std::shared_ptr<core::actor::Mailbox> mailbox,
                 core::actor::ActorSystem& actorSystem,
                 std::shared_ptr<spdlog::logger> logger);

    void handle(core::actor::ActorEnvelope&& envelope) override;

    void bindTransport(const std::string& sessionId,
                       std::weak_ptr<network::websocket::SessionTransport> transport);
    void unbindTransport(const std::string& sessionId);

private:
    core::actor::ActorSystem& actorSystem_;
    std::shared_ptr<spdlog::logger> logger_;
    mutable std::shared_mutex mutex_;
    std::unordered_map<std::string, std::weak_ptr<network::websocket::SessionTransport>> transports_;
};

} // namespace svitock::infrastructure::actors
