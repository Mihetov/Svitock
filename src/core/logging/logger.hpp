#pragma once

#include <memory>

#include <spdlog/logger.h>

namespace svitock::core::logging {

class LoggerFactory {
public:
    static std::shared_ptr<spdlog::logger> createDefault(const std::string& level);
};

} // namespace svitock::core::logging
