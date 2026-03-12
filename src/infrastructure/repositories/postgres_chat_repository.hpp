#pragma once

#include "application/interfaces/chat_repository.hpp"
#include "infrastructure/database/postgres_pool.hpp"

namespace svitock::infrastructure::repositories {

class PostgresChatRepository final : public application::interfaces::ChatRepository {
public:
    explicit PostgresChatRepository(database::PostgresPool& pool) noexcept;

    bool create(const domain::chat::Chat& chat) override;
    std::optional<domain::chat::Chat> findById(const boost::uuids::uuid& id) const override;

private:
    database::PostgresPool& pool_;
};

} // namespace svitock::infrastructure::repositories
