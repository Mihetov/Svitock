#include "core/actor/mailbox.hpp"

namespace svitock::core::actor {

Mailbox::Mailbox(const std::size_t capacity)
    : queue_(capacity) {}

Mailbox::~Mailbox() {
    active_.store(false, std::memory_order_release);
    ActorEnvelope* raw = nullptr;
    while (queue_.pop(raw)) {
        std::unique_ptr<ActorEnvelope> guard(raw);
    }
}

bool Mailbox::push(std::unique_ptr<ActorEnvelope> message) {
    if (!active_.load(std::memory_order_acquire)) {
        return false;
    }
    return queue_.bounded_push(message.release());
}

std::unique_ptr<ActorEnvelope> Mailbox::pop() {
    ActorEnvelope* raw = nullptr;
    if (!queue_.pop(raw)) {
        return nullptr;
    }
    return std::unique_ptr<ActorEnvelope>(raw);
}

} // namespace svitock::core::actor
