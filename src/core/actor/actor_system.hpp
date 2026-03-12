#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

#include <spdlog/logger.h>

#include "core/actor/actor.hpp"

namespace svitock::core::actor {

class ActorSystem {
public:
    explicit ActorSystem(std::shared_ptr<spdlog::logger> logger);
    ~ActorSystem();

    void registerActor(ActorType actorType, std::shared_ptr<Actor> actor);
    bool send(std::unique_ptr<ActorEnvelope> envelope) const;

    void start(std::size_t workerThreads);
    void stop();

private:
    void workerLoop();

    struct ActorTypeHash {
        std::size_t operator()(const ActorType type) const noexcept {
            return static_cast<std::size_t>(type);
        }
    };

    std::unordered_map<ActorType, std::shared_ptr<Actor>, ActorTypeHash> actors_;
    std::shared_ptr<spdlog::logger> logger_;
    std::vector<std::thread> workers_;
    std::atomic<bool> running_{false};
};

} // namespace svitock::core::actor
