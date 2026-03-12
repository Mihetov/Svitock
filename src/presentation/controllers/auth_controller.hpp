#pragma once

#include <string>

#include "application/use_cases/login_user_use_case.hpp"
#include "application/use_cases/register_user_use_case.hpp"

namespace svitock::presentation::controllers {

class AuthController {
public:
    AuthController(application::use_cases::RegisterUserUseCase& registerUser,
                   application::use_cases::LoginUserUseCase& loginUser) noexcept;

    bool registerUserByPassword(const std::string& username, const std::string& passwordHash) const;
    bool loginUserByPassword(const std::string& username, const std::string& passwordHash) const;

private:
    application::use_cases::RegisterUserUseCase& registerUser_;
    application::use_cases::LoginUserUseCase& loginUser_;
};

} // namespace svitock::presentation::controllers
