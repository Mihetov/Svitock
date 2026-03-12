#include "infrastructure/network/server.hpp"

#include <boost/asio/ip/address.hpp>

#include "infrastructure/actors/chat_actor.hpp"
#include "infrastructure/actors/message_actor.hpp"
#include "infrastructure/actors/session_actor.hpp"
#include "infrastructure/actors/storage_actor.hpp"
#include "infrastructure/dispatcher/message_dispatcher.hpp"
#include "infrastructure/network/http/http_listener.hpp"
#include "infrastructure/network/http/http_router.hpp"
#include "infrastructure/network/websocket/websocket_listener.hpp"

namespace svitock::infrastructure::network {

Server::Server(const core::config::AppConfig& config,
               application::interfaces::MessageRepository& messageRepository,
               std::shared_ptr<spdlog::logger> logger)
    : config_(config),
      logger_(std::move(logger)),
      networkThreadPool_(networkIoContext_),
      actorSystem_(logger_),
      jwtService_(std::make_unique<core::security::JwtService>(config.security.jwtSecret)) {
    const auto mailboxSize = config_.server.mailboxCapacity;
    sessionActor_ = std::make_shared<actors::SessionActor>(
        std::make_shared<core::actor::Mailbox>(mailboxSize), actorSystem_, logger_);
    chatActor_ = std::make_shared<actors::ChatActor>(
        std::make_shared<core::actor::Mailbox>(mailboxSize), actorSystem_, logger_);
    messageActor_ = std::make_shared<actors::MessageActor>(
        std::make_shared<core::actor::Mailbox>(mailboxSize), actorSystem_, logger_);
    storageActor_ = std::make_shared<actors::StorageActor>(
        std::make_shared<core::actor::Mailbox>(mailboxSize), messageRepository, logger_);

    actorSystem_.registerActor(core::actor::ActorType::session, sessionActor_);
    actorSystem_.registerActor(core::actor::ActorType::chat, chatActor_);
    actorSystem_.registerActor(core::actor::ActorType::message, messageActor_);
    actorSystem_.registerActor(core::actor::ActorType::storage, storageActor_);

    dispatcher_ = std::make_shared<dispatcher::MessageDispatcher>(actorSystem_, logger_);

    auto address = boost::asio::ip::make_address(config_.server.host);
    websocketListener_ = std::make_shared<websocket::WebSocketListener>(
        networkIoContext_,
        boost::asio::ip::tcp::endpoint(address, config_.server.wsPort),
        *dispatcher_,
        *sessionActor_,
        logger_);

    httpRouter_ = std::make_shared<http::HttpRouter>(messageRepository, *jwtService_);
    httpListener_ = std::make_shared<http::HttpListener>(
        networkIoContext_,
        boost::asio::ip::tcp::endpoint(address, config_.server.httpPort),
        *httpRouter_);
}

void Server::run() {
    if (running_.exchange(true)) {
        return;
    }

    websocketListener_->run();
    httpListener_->run();
    actorSystem_.start(config_.server.actorThreads);
    networkThreadPool_.start(config_.server.networkThreads);

    logger_->info("server started ws={} http={} net_threads={} actor_threads={}",
                  config_.server.wsPort,
                  config_.server.httpPort,
                  config_.server.networkThreads,
                  config_.server.actorThreads);
}

void Server::stop() {
    if (!running_.exchange(false)) {
        return;
    }

    networkThreadPool_.stop();
    actorSystem_.stop();
    logger_->info("server stopped");
}

} // namespace svitock::infrastructure::network
