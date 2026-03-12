#pragma once

#include <string>

#include "application/interfaces/user_repository.hpp"

namespace svitock::application::use_cases {

class RegisterUserUseCase {
public:
    explicit RegisterUserUseCase(interfaces::UserRepository& userRepository) noexcept;

    bool execute(const std::string& username, const std::string& passwordHash) const;

private:
    interfaces::UserRepository& userRepository_;
};

} // namespace svitock::application::use_cases
