#pragma once

#include <cstdint>
#include <string>
#include <variant>

#include <nlohmann/json.hpp>

namespace svitock::core::actor {

enum class ActorType : std::uint8_t {
    session,
    chat,
    message,
    storage,
};

struct AuthEvent {
    std::string sessionId;
    std::string userId;
};

struct SendMessageEvent {
    std::string sessionId;
    std::string chatId;
    std::string senderId;
    std::string recipientId;
    std::string text;
};

struct TypingEvent {
    std::string chatId;
    std::string userId;
};

struct ReadEvent {
    std::string chatId;
    std::string userId;
    std::string messageId;
};

struct StoragePersistMessage {
    std::string chatId;
    std::string senderId;
    std::string content;
};

struct SessionOutgoingEvent {
    std::string sessionId;
    nlohmann::json payload;
};

using ActorPayload = std::variant<AuthEvent,
                                  SendMessageEvent,
                                  TypingEvent,
                                  ReadEvent,
                                  StoragePersistMessage,
                                  SessionOutgoingEvent>;

struct ActorEnvelope {
    ActorType destination;
    ActorPayload payload;
};

} // namespace svitock::core::actor
