#pragma once

#include <memory>

#include <spdlog/logger.h>

#include "core/actor/actor.hpp"

namespace svitock::core::actor {
class ActorSystem;
}

namespace svitock::infrastructure::actors {

class MessageActor final : public core::actor::Actor {
public:
    MessageActor(std::shared_ptr<core::actor::Mailbox> mailbox,
                 core::actor::ActorSystem& actorSystem,
                 std::shared_ptr<spdlog::logger> logger);

    void handle(core::actor::ActorEnvelope&& envelope) override;

private:
    core::actor::ActorSystem& actorSystem_;
    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace svitock::infrastructure::actors
