#include <condition_variable>
#include <csignal>
#include <cstdlib>
#include <mutex>

#include "core/config/app_config.hpp"
#include "core/logging/logger.hpp"
#include "infrastructure/database/postgres_pool.hpp"
#include "infrastructure/network/server.hpp"
#include "infrastructure/repositories/postgres_message_repository.hpp"

namespace {
std::mutex mutex;
std::condition_variable cv;
bool stopped = false;

void onSignal(int) {
    {
        std::scoped_lock lock(mutex);
        stopped = true;
    }
    cv.notify_all();
}
} // namespace

int main() {
    const auto config = svitock::core::config::ConfigLoader::fromFile("config/server.yaml");
    auto logger = svitock::core::logging::LoggerFactory::createDefault(config.logging.level);

    const std::string conn =
        "host=" + config.database.host +
        " port=" + std::to_string(config.database.port) +
        " user=" + config.database.user +
        " password=" + config.database.password +
        " dbname=" + config.database.db;

    svitock::infrastructure::database::PostgresPool pool(conn, config.database.poolSize);
    svitock::infrastructure::repositories::PostgresMessageRepository messageRepository(pool);

    svitock::infrastructure::network::Server server(config, messageRepository, logger);

    std::signal(SIGINT, onSignal);
    std::signal(SIGTERM, onSignal);

    server.run();

    std::unique_lock lock(mutex);
    cv.wait(lock, [] { return stopped; });

    server.stop();
    return EXIT_SUCCESS;
}
