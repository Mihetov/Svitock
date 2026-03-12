#pragma once

#include <atomic>
#include <memory>

#include <boost/asio/io_context.hpp>

#include "application/interfaces/message_repository.hpp"
#include "core/actor/actor_system.hpp"
#include "core/config/app_config.hpp"
#include "core/security/jwt_service.hpp"
#include "core/threading/thread_pool.hpp"

namespace spdlog {
class logger;
}

namespace svitock::infrastructure::actors {
class ChatActor;
class MessageActor;
class SessionActor;
class StorageActor;
}

namespace svitock::infrastructure::dispatcher {
class MessageDispatcher;
}

namespace svitock::infrastructure::network::http {
class HttpListener;
class HttpRouter;
}

namespace svitock::infrastructure::network::websocket {
class WebSocketListener;
}

namespace svitock::infrastructure::network {

class Server {
public:
    Server(const core::config::AppConfig& config,
           application::interfaces::MessageRepository& messageRepository,
           std::shared_ptr<spdlog::logger> logger);

    void run();
    void stop();

private:
    core::config::AppConfig config_;
    std::shared_ptr<spdlog::logger> logger_;

    boost::asio::io_context networkIoContext_;
    core::threading::ThreadPool networkThreadPool_;

    core::actor::ActorSystem actorSystem_;
    std::unique_ptr<core::security::JwtService> jwtService_;

    std::shared_ptr<actors::SessionActor> sessionActor_;
    std::shared_ptr<actors::ChatActor> chatActor_;
    std::shared_ptr<actors::MessageActor> messageActor_;
    std::shared_ptr<actors::StorageActor> storageActor_;

    std::shared_ptr<dispatcher::MessageDispatcher> dispatcher_;
    std::shared_ptr<websocket::WebSocketListener> websocketListener_;
    std::shared_ptr<http::HttpRouter> httpRouter_;
    std::shared_ptr<http::HttpListener> httpListener_;

    std::atomic<bool> running_{false};
};

} // namespace svitock::infrastructure::network
