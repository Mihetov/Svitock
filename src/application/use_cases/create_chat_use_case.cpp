#include "application/use_cases/create_chat_use_case.hpp"

#include <boost/uuid/random_generator.hpp>

namespace svitock::application::use_cases {

CreateChatUseCase::CreateChatUseCase(interfaces::ChatRepository& chatRepository) noexcept
    : chatRepository_(chatRepository) {}

bool CreateChatUseCase::execute(domain::chat::ChatType type) const {
    const domain::chat::Chat chat{boost::uuids::random_generator{}(), type, std::chrono::system_clock::now()};
    return chatRepository_.create(chat);
}

} // namespace svitock::application::use_cases
