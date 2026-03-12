#include "application/use_cases/send_message_use_case.hpp"

#include <boost/uuid/random_generator.hpp>

namespace svitock::application::use_cases {

SendMessageUseCase::SendMessageUseCase(interfaces::MessageRepository& messageRepository) noexcept
    : messageRepository_(messageRepository) {}

bool SendMessageUseCase::execute(const boost::uuids::uuid& chatId,
                                 const boost::uuids::uuid& senderId,
                                 const std::string& content) const {
    if (content.empty()) {
        return false;
    }
    const domain::message::Message message{
        boost::uuids::random_generator{}(),
        chatId,
        senderId,
        content,
        std::chrono::system_clock::now()};
    return messageRepository_.create(message);
}

} // namespace svitock::application::use_cases
