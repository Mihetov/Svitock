#include "core/config/app_config.hpp"

#include <yaml-cpp/yaml.h>

namespace svitock::core::config {

AppConfig ConfigLoader::fromFile(const std::string& path) {
    const YAML::Node root = YAML::LoadFile(path);
    AppConfig cfg;

    cfg.server.host = root["server"]["host"].as<std::string>(cfg.server.host);
    cfg.server.wsPort = root["server"]["ws_port"].as<std::uint16_t>(cfg.server.wsPort);
    cfg.server.httpPort = root["server"]["http_port"].as<std::uint16_t>(cfg.server.httpPort);
    cfg.server.networkThreads = root["server"]["network_threads"].as<std::size_t>(cfg.server.networkThreads);
    cfg.server.actorThreads = root["server"]["actor_threads"].as<std::size_t>(cfg.server.actorThreads);
    cfg.server.storageThreads = root["server"]["storage_threads"].as<std::size_t>(cfg.server.storageThreads);
    cfg.server.mailboxCapacity = root["server"]["mailbox_capacity"].as<std::size_t>(cfg.server.mailboxCapacity);

    cfg.database.host = root["database"]["host"].as<std::string>();
    cfg.database.port = root["database"]["port"].as<std::uint16_t>();
    cfg.database.user = root["database"]["user"].as<std::string>();
    cfg.database.password = root["database"]["password"].as<std::string>();
    cfg.database.db = root["database"]["db"].as<std::string>();
    cfg.database.poolSize = root["database"]["pool_size"].as<std::uint16_t>(cfg.database.poolSize);

    cfg.security.jwtSecret = root["security"]["jwt_secret"].as<std::string>(cfg.security.jwtSecret);
    cfg.security.jwtTtlSeconds = root["security"]["jwt_ttl_seconds"].as<std::uint64_t>(cfg.security.jwtTtlSeconds);

    cfg.logging.level = root["logging"]["level"].as<std::string>(cfg.logging.level);
    return cfg;
}

} // namespace svitock::core::config
