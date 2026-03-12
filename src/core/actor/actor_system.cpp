#include "core/actor/actor_system.hpp"

#include <chrono>

namespace svitock::core::actor {

ActorSystem::ActorSystem(std::shared_ptr<spdlog::logger> logger)
    : logger_(std::move(logger)) {}

ActorSystem::~ActorSystem() {
    stop();
}

void ActorSystem::registerActor(const ActorType actorType, std::shared_ptr<Actor> actor) {
    actors_[actorType] = std::move(actor);
}

bool ActorSystem::send(std::unique_ptr<ActorEnvelope> envelope) const {
    const auto it = actors_.find(envelope->destination);
    if (it == actors_.end()) {
        return false;
    }

    auto mailbox = it->second->mailbox();
    return mailbox->push(std::move(envelope));
}

void ActorSystem::start(const std::size_t workerThreads) {
    if (running_.exchange(true)) {
        return;
    }

    workers_.reserve(workerThreads);
    for (std::size_t i = 0; i < workerThreads; ++i) {
        workers_.emplace_back([this] { workerLoop(); });
    }
}

void ActorSystem::stop() {
    if (!running_.exchange(false)) {
        return;
    }

    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    workers_.clear();
}

void ActorSystem::workerLoop() {
    while (running_.load(std::memory_order_acquire)) {
        bool processed = false;
        for (const auto& [_, actor] : actors_) {
            if (auto envelope = actor->mailbox()->pop(); envelope != nullptr) {
                actor->handle(std::move(*envelope));
                processed = true;
            }
        }
        if (!processed) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    logger_->info("actor worker stopped");
}

} // namespace svitock::core::actor
