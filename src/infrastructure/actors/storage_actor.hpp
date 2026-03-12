#pragma once

#include <memory>

#include <spdlog/logger.h>

#include "application/interfaces/message_repository.hpp"
#include "core/actor/actor.hpp"

namespace svitock::infrastructure::actors {

class StorageActor final : public core::actor::Actor {
public:
    StorageActor(std::shared_ptr<core::actor::Mailbox> mailbox,
                 application::interfaces::MessageRepository& messageRepository,
                 std::shared_ptr<spdlog::logger> logger);

    void handle(core::actor::ActorEnvelope&& envelope) override;

private:
    application::interfaces::MessageRepository& messageRepository_;
    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace svitock::infrastructure::actors
