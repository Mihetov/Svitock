#pragma once

#include <chrono>
#include <string>

#include <boost/uuid/uuid.hpp>

namespace svitock::domain::user {

class User {
public:
    User(boost::uuids::uuid id,
         std::string username,
         std::string passwordHash,
         std::chrono::system_clock::time_point createdAt) noexcept;

    [[nodiscard]] const boost::uuids::uuid& getId() const noexcept;
    [[nodiscard]] const std::string& getUsername() const noexcept;
    [[nodiscard]] const std::string& getPasswordHash() const noexcept;
    [[nodiscard]] std::chrono::system_clock::time_point getCreatedAt() const noexcept;

private:
    boost::uuids::uuid id_;
    std::string username_;
    std::string passwordHash_;
    std::chrono::system_clock::time_point createdAt_;
};

} // namespace svitock::domain::user
