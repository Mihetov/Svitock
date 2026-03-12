#include "application/use_cases/login_user_use_case.hpp"

namespace svitock::application::use_cases {

LoginUserUseCase::LoginUserUseCase(const interfaces::UserRepository& userRepository) noexcept
    : userRepository_(userRepository) {}

bool LoginUserUseCase::execute(const std::string& username, const std::string& passwordHash) const {
    const auto user = userRepository_.findByUsername(username);
    if (!user.has_value()) {
        return false;
    }
    return user->getPasswordHash() == passwordHash;
}

} // namespace svitock::application::use_cases
