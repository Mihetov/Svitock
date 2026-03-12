#include "application/use_cases/register_user_use_case.hpp"

#include <boost/uuid/random_generator.hpp>

namespace svitock::application::use_cases {

RegisterUserUseCase::RegisterUserUseCase(interfaces::UserRepository& userRepository) noexcept
    : userRepository_(userRepository) {}

bool RegisterUserUseCase::execute(const std::string& username, const std::string& passwordHash) const {
    if (username.empty() || passwordHash.empty()) {
        return false;
    }
    if (userRepository_.findByUsername(username).has_value()) {
        return false;
    }
    const auto now = std::chrono::system_clock::now();
    const domain::user::User user{boost::uuids::random_generator{}(), username, passwordHash, now};
    return userRepository_.create(user);
}

} // namespace svitock::application::use_cases
