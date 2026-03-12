#include "infrastructure/actors/storage_actor.hpp"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>

#include "domain/message/message.hpp"

namespace svitock::infrastructure::actors {

StorageActor::StorageActor(std::shared_ptr<core::actor::Mailbox> mailbox,
                           application::interfaces::MessageRepository& messageRepository,
                           std::shared_ptr<spdlog::logger> logger)
    : Actor(std::move(mailbox)), messageRepository_(messageRepository), logger_(std::move(logger)) {}

void StorageActor::handle(core::actor::ActorEnvelope&& envelope) {
    if (!std::holds_alternative<core::actor::StoragePersistMessage>(envelope.payload)) {
        return;
    }

    const auto& persist = std::get<core::actor::StoragePersistMessage>(envelope.payload);
    const domain::message::Message message{
        boost::uuids::random_generator{}(),
        boost::uuids::string_generator{}(persist.chatId),
        boost::uuids::string_generator{}(persist.senderId),
        persist.content,
        std::chrono::system_clock::now()};

    const bool stored = messageRepository_.create(message);
    if (!stored) {
        logger_->error("storage_actor failed to persist message chat_id={}", persist.chatId);
    }
}

} // namespace svitock::infrastructure::actors
