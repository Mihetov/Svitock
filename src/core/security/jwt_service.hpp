#pragma once

#include <cstdint>
#include <string>

namespace svitock::core::security {

class JwtService {
public:
    explicit JwtService(std::string secret) noexcept;

    [[nodiscard]] std::string issueToken(const std::string& userId, std::uint64_t ttlSeconds) const;
    [[nodiscard]] bool validateToken(const std::string& token) const;
    [[nodiscard]] std::string extractUserId(const std::string& token) const;

private:
    [[nodiscard]] std::string sign(const std::string& data) const;

    std::string secret_;
};

} // namespace svitock::core::security
