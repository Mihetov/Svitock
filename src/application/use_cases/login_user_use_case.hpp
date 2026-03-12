#pragma once

#include <string>

#include "application/interfaces/user_repository.hpp"

namespace svitock::application::use_cases {

class LoginUserUseCase {
public:
    explicit LoginUserUseCase(const interfaces::UserRepository& userRepository) noexcept;

    bool execute(const std::string& username, const std::string& passwordHash) const;

private:
    const interfaces::UserRepository& userRepository_;
};

} // namespace svitock::application::use_cases
