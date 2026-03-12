#pragma once

#include <atomic>
#include <cstddef>
#include <memory>

#include <boost/lockfree/queue.hpp>

#include "core/actor/actor_message.hpp"

namespace svitock::core::actor {

class Mailbox {
public:
    explicit Mailbox(std::size_t capacity);
    ~Mailbox();

    Mailbox(const Mailbox&) = delete;
    Mailbox& operator=(const Mailbox&) = delete;

    bool push(std::unique_ptr<ActorEnvelope> message);
    std::unique_ptr<ActorEnvelope> pop();

private:
    boost::lockfree::queue<ActorEnvelope*> queue_;
    std::atomic<bool> active_{true};
};

} // namespace svitock::core::actor
