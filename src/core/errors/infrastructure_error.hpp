#pragma once

#include <stdexcept>
#include <string>

namespace svitock::core::errors {

class InfrastructureError : public std::runtime_error {
public:
    explicit InfrastructureError(const std::string& message)
        : std::runtime_error(message) {}
};

} // namespace svitock::core::errors
