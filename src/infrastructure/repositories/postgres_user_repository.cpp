#include "infrastructure/repositories/postgres_user_repository.hpp"

#include <boost/uuid/uuid_io.hpp>

namespace svitock::infrastructure::repositories {

PostgresUserRepository::PostgresUserRepository(database::PostgresPool& pool) noexcept
    : pool_(pool) {}

std::optional<domain::user::User> PostgresUserRepository::findByUsername(const std::string& username) const {
    auto connection = pool_.acquire();
    pqxx::work transaction(*connection);
    const auto result = transaction.exec_params(
        "SELECT id, username, password_hash FROM users WHERE username = $1",
        username);
    if (result.empty()) {
        return std::nullopt;
    }

    return domain::user::User{
        boost::uuids::string_generator{}(result[0]["id"].as<std::string>()),
        result[0]["username"].as<std::string>(),
        result[0]["password_hash"].as<std::string>(),
        std::chrono::system_clock::now()};
}

bool PostgresUserRepository::create(const domain::user::User& user) {
    auto connection = pool_.acquire();
    pqxx::work transaction(*connection);
    transaction.exec_params(
        "INSERT INTO users (id, username, password_hash, created_at) VALUES ($1, $2, $3, NOW())",
        boost::uuids::to_string(user.getId()),
        user.getUsername(),
        user.getPasswordHash());
    transaction.commit();
    return true;
}

} // namespace svitock::infrastructure::repositories
