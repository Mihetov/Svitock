#pragma once

#include <optional>
#include <string>

#include "domain/user/user.hpp"

namespace svitock::application::interfaces {

class UserRepository {
public:
    virtual ~UserRepository() = default;

    virtual std::optional<domain::user::User> findByUsername(const std::string& username) const = 0;
    virtual bool create(const domain::user::User& user) = 0;
};

} // namespace svitock::application::interfaces
