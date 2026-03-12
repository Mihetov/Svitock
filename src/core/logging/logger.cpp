#include "core/logging/logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace svitock::core::logging {

std::shared_ptr<spdlog::logger> LoggerFactory::createDefault(const std::string& level) {
    constexpr const char* loggerName = "svitock";
    if (auto existing = spdlog::get(loggerName); existing != nullptr) {
        existing->set_level(spdlog::level::from_str(level));
        return existing;
    }

    auto logger = spdlog::stdout_color_mt(loggerName);
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");
    logger->set_level(spdlog::level::from_str(level));
    return logger;
}

} // namespace svitock::core::logging
