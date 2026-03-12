#pragma once

#include <memory>

#include "core/actor/actor_message.hpp"
#include "core/actor/mailbox.hpp"

namespace svitock::core::actor {

class Actor {
public:
    explicit Actor(std::shared_ptr<Mailbox> mailbox) noexcept
        : mailbox_(std::move(mailbox)) {}
    virtual ~Actor() = default;

    virtual void handle(ActorEnvelope&& envelope) = 0;

    [[nodiscard]] std::shared_ptr<Mailbox> mailbox() const noexcept { return mailbox_; }

private:
    std::shared_ptr<Mailbox> mailbox_;
};

} // namespace svitock::core::actor
