#include "core/security/jwt_service.hpp"

#include <chrono>
#include <sstream>
#include <string_view>

#include <boost/uuid/detail/sha1.hpp>
#include <nlohmann/json.hpp>

namespace svitock::core::security {

namespace {
std::string toHex(const unsigned int value) {
    std::ostringstream stream;
    stream << std::hex << value;
    return stream.str();
}

std::string toCompactJson(const nlohmann::json& value) {
    return value.dump(-1, ' ', false, nlohmann::json::error_handler_t::replace);
}
} // namespace

JwtService::JwtService(std::string secret) noexcept
    : secret_(std::move(secret)) {}

std::string JwtService::issueToken(const std::string& userId, const std::uint64_t ttlSeconds) const {
    const auto now = std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now().time_since_epoch())
                         .count();
    const nlohmann::json header{{"alg", "HS1"}, {"typ", "JWT"}};
    const nlohmann::json payload{{"sub", userId}, {"iat", now}, {"exp", now + ttlSeconds}};

    const auto raw = toCompactJson(header) + "." + toCompactJson(payload);
    return raw + "." + sign(raw);
}

bool JwtService::validateToken(const std::string& token) const {
    const auto lastDot = token.rfind('.');
    if (lastDot == std::string::npos) {
        return false;
    }

    const auto data = token.substr(0, lastDot);
    const auto signature = token.substr(lastDot + 1);
    return sign(data) == signature;
}

std::string JwtService::extractUserId(const std::string& token) const {
    const auto firstDot = token.find('.');
    const auto lastDot = token.rfind('.');
    if (firstDot == std::string::npos || lastDot == std::string::npos || firstDot == lastDot) {
        return {};
    }

    const auto payloadRaw = token.substr(firstDot + 1, lastDot - firstDot - 1);
    const auto payload = nlohmann::json::parse(payloadRaw, nullptr, false);
    if (!payload.is_object()) {
        return {};
    }
    return payload.value("sub", "");
}

std::string JwtService::sign(const std::string& data) const {
    boost::uuids::detail::sha1 hasher;
    hasher.process_bytes(data.data(), data.size());
    hasher.process_bytes(secret_.data(), secret_.size());

    unsigned int digest[5]{};
    hasher.get_digest(digest);

    std::string signature;
    signature.reserve(40);
    for (const auto part : digest) {
        signature += toHex(part);
    }
    return signature;
}

} // namespace svitock::core::security
