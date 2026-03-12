#include "infrastructure/repositories/postgres_message_repository.hpp"

#include <boost/uuid/uuid_io.hpp>

namespace svitock::infrastructure::repositories {

PostgresMessageRepository::PostgresMessageRepository(database::PostgresPool& pool) noexcept
    : pool_(pool) {}

bool PostgresMessageRepository::create(const domain::message::Message& message) {
    auto connection = pool_.acquire();
    pqxx::work transaction(*connection);
    transaction.exec_params(
        "INSERT INTO messages (id, chat_id, sender_id, content, created_at) VALUES ($1, $2, $3, $4, NOW())",
        boost::uuids::to_string(message.getId()),
        boost::uuids::to_string(message.getChatId()),
        boost::uuids::to_string(message.getSenderId()),
        message.getContent());
    transaction.commit();
    return true;
}

std::vector<domain::message::Message> PostgresMessageRepository::getByChat(const boost::uuids::uuid& chatId,
                                                                            const std::size_t limit) const {
    auto connection = pool_.acquire();
    pqxx::work transaction(*connection);
    const auto result = transaction.exec_params(
        "SELECT id, chat_id, sender_id, content FROM messages WHERE chat_id = $1 ORDER BY created_at DESC LIMIT $2",
        boost::uuids::to_string(chatId),
        limit);

    std::vector<domain::message::Message> messages;
    messages.reserve(result.size());
    for (const auto& row : result) {
        messages.emplace_back(
            boost::uuids::string_generator{}(row["id"].as<std::string>()),
            boost::uuids::string_generator{}(row["chat_id"].as<std::string>()),
            boost::uuids::string_generator{}(row["sender_id"].as<std::string>()),
            row["content"].as<std::string>(),
            std::chrono::system_clock::now());
    }
    return messages;
}

} // namespace svitock::infrastructure::repositories
