#pragma once

#include "application/interfaces/user_repository.hpp"
#include "infrastructure/database/postgres_pool.hpp"

namespace svitock::infrastructure::repositories {

class PostgresUserRepository final : public application::interfaces::UserRepository {
public:
    explicit PostgresUserRepository(database::PostgresPool& pool) noexcept;

    std::optional<domain::user::User> findByUsername(const std::string& username) const override;
    bool create(const domain::user::User& user) override;

private:
    database::PostgresPool& pool_;
};

} // namespace svitock::infrastructure::repositories
