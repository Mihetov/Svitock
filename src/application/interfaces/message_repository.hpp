#pragma once

#include <vector>

#include "domain/message/message.hpp"

namespace svitock::application::interfaces {

class MessageRepository {
public:
    virtual ~MessageRepository() = default;

    virtual bool create(const domain::message::Message& message) = 0;
    virtual std::vector<domain::message::Message> getByChat(const boost::uuids::uuid& chatId,
                                                            std::size_t limit) const = 0;
};

} // namespace svitock::application::interfaces
