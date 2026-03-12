#include "infrastructure/repositories/postgres_chat_repository.hpp"

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace svitock::infrastructure::repositories {

PostgresChatRepository::PostgresChatRepository(database::PostgresPool& pool) noexcept
    : pool_(pool) {}

bool PostgresChatRepository::create(const domain::chat::Chat& chat) {
    auto connection = pool_.acquire();
    pqxx::work transaction(*connection);
    transaction.exec_params(
        "INSERT INTO chats (id, type, created_at) VALUES ($1, $2, NOW())",
        boost::uuids::to_string(chat.getId()),
        static_cast<std::int32_t>(chat.getType()));
    transaction.commit();
    return true;
}

std::optional<domain::chat::Chat> PostgresChatRepository::findById(const boost::uuids::uuid& id) const {
    auto connection = pool_.acquire();
    pqxx::work transaction(*connection);
    const auto result = transaction.exec_params("SELECT id, type FROM chats WHERE id = $1", boost::uuids::to_string(id));
    if (result.empty()) {
        return std::nullopt;
    }

    boost::uuids::string_generator generator;
    return domain::chat::Chat(
        generator(result[0]["id"].as<std::string>()),
        static_cast<domain::chat::ChatType>(result[0]["type"].as<std::int32_t>()),
        std::chrono::system_clock::now());
}

} // namespace svitock::infrastructure::repositories
