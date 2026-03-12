#include "domain/user/user.hpp"

namespace svitock::domain::user {

User::User(boost::uuids::uuid id,
           std::string username,
           std::string passwordHash,
           std::chrono::system_clock::time_point createdAt) noexcept
    : id_(id), username_(std::move(username)), passwordHash_(std::move(passwordHash)), createdAt_(createdAt) {}

const boost::uuids::uuid& User::getId() const noexcept { return id_; }

const std::string& User::getUsername() const noexcept { return username_; }

const std::string& User::getPasswordHash() const noexcept { return passwordHash_; }

std::chrono::system_clock::time_point User::getCreatedAt() const noexcept { return createdAt_; }

} // namespace svitock::domain::user
