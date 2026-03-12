#pragma once

#include <cstdint>
#include <string>

namespace svitock::core::config {

struct ServerConfig {
    std::string host{"0.0.0.0"};
    std::uint16_t wsPort{8080};
    std::uint16_t httpPort{8081};
    std::size_t networkThreads{4};
    std::size_t actorThreads{8};
    std::size_t storageThreads{4};
    std::size_t mailboxCapacity{65536};
};

struct DatabaseConfig {
    std::string host;
    std::uint16_t port{};
    std::string user;
    std::string password;
    std::string db;
    std::uint16_t poolSize{16};
};

struct SecurityConfig {
    std::string jwtSecret{"change-me"};
    std::uint64_t jwtTtlSeconds{3600};
};

struct LoggingConfig {
    std::string level{"info"};
};

struct AppConfig {
    ServerConfig server;
    DatabaseConfig database;
    SecurityConfig security;
    LoggingConfig logging;
};

class ConfigLoader {
public:
    static AppConfig fromFile(const std::string& path);
};

} // namespace svitock::core::config
