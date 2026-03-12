#pragma once

#include "application/interfaces/message_repository.hpp"
#include "infrastructure/database/postgres_pool.hpp"

namespace svitock::infrastructure::repositories {

class PostgresMessageRepository final : public application::interfaces::MessageRepository {
public:
    explicit PostgresMessageRepository(database::PostgresPool& pool) noexcept;

    bool create(const domain::message::Message& message) override;
    std::vector<domain::message::Message> getByChat(const boost::uuids::uuid& chatId,
                                                    std::size_t limit) const override;

private:
    database::PostgresPool& pool_;
};

} // namespace svitock::infrastructure::repositories
