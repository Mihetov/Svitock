#include "infrastructure/database/postgres_pool.hpp"

#include "core/errors/infrastructure_error.hpp"

namespace svitock::infrastructure::database {

PostgresPool::PostgresPool(const std::string& connectionString, const std::size_t poolSize) {
    if (poolSize == 0) {
        throw core::errors::InfrastructureError("postgres pool size must be > 0");
    }

    pool_.reserve(poolSize);
    for (std::size_t i = 0; i < poolSize; ++i) {
        auto connection = std::make_shared<pqxx::connection>(connectionString);
        if (!connection->is_open()) {
            throw core::errors::InfrastructureError("failed to open postgres connection");
        }
        pool_.push_back(std::move(connection));
    }
}

std::shared_ptr<pqxx::connection> PostgresPool::acquire() const {
    const auto index = next_.fetch_add(1, std::memory_order_relaxed) % pool_.size();
    return pool_[index];
}

} // namespace svitock::infrastructure::database
