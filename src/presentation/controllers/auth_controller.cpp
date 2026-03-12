#include "presentation/controllers/auth_controller.hpp"

namespace svitock::presentation::controllers {

AuthController::AuthController(application::use_cases::RegisterUserUseCase& registerUser,
                               application::use_cases::LoginUserUseCase& loginUser) noexcept
    : registerUser_(registerUser), loginUser_(loginUser) {}

bool AuthController::registerUserByPassword(const std::string& username, const std::string& passwordHash) const {
    return registerUser_.execute(username, passwordHash);
}

bool AuthController::loginUserByPassword(const std::string& username, const std::string& passwordHash) const {
    return loginUser_.execute(username, passwordHash);
}

} // namespace svitock::presentation::controllers
