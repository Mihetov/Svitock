#pragma once

#include <string>

#include <nlohmann/json.hpp>

namespace svitock::presentation::dto {

struct MessageEventDto {
    std::string type;
    std::string chatId;
    nlohmann::json payload;
};

inline void to_json(nlohmann::json& j, const MessageEventDto& dto) {
    j = nlohmann::json{{"type", dto.type}, {"chat_id", dto.chatId}, {"payload", dto.payload}};
}

inline void from_json(const nlohmann::json& j, MessageEventDto& dto) {
    j.at("type").get_to(dto.type);
    j.at("chat_id").get_to(dto.chatId);
    j.at("payload").get_to(dto.payload);
}

} // namespace svitock::presentation::dto
