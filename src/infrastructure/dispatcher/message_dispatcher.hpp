#pragma once

#include <memory>
#include <string>

#include <spdlog/logger.h>

#include "core/actor/actor_system.hpp"

namespace svitock::infrastructure::dispatcher {

class MessageDispatcher {
public:
    MessageDispatcher(core::actor::ActorSystem& actorSystem,
                      std::shared_ptr<spdlog::logger> logger) noexcept;

    void dispatch(const std::string& sessionId, const std::string& rawMessage) const;

private:
    core::actor::ActorSystem& actorSystem_;
    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace svitock::infrastructure::dispatcher
