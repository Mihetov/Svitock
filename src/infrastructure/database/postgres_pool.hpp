#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <vector>

#include <pqxx/pqxx>

namespace svitock::infrastructure::database {

class PostgresPool {
public:
    PostgresPool(const std::string& connectionString, std::size_t poolSize);

    [[nodiscard]] std::shared_ptr<pqxx::connection> acquire() const;

private:
    std::vector<std::shared_ptr<pqxx::connection>> pool_;
    mutable std::atomic<std::size_t> next_{0};
};

} // namespace svitock::infrastructure::database
