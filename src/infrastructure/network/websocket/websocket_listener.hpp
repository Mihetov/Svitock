#pragma once

#include <memory>

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/error.hpp>
#include <spdlog/logger.h>

namespace svitock::infrastructure::dispatcher {
class MessageDispatcher;
}

namespace svitock::infrastructure::actors {
class SessionActor;
}

namespace svitock::infrastructure::network::websocket {

class WebSocketListener final : public std::enable_shared_from_this<WebSocketListener> {
public:
    WebSocketListener(boost::asio::io_context& ioContext,
                      boost::asio::ip::tcp::endpoint endpoint,
                      dispatcher::MessageDispatcher& dispatcher,
                      actors::SessionActor& sessionActor,
                      std::shared_ptr<spdlog::logger> logger);

    void run();

private:
    void doAccept();
    void onAccept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);

    boost::asio::ip::tcp::acceptor acceptor_;
    dispatcher::MessageDispatcher& dispatcher_;
    actors::SessionActor& sessionActor_;
    std::shared_ptr<spdlog::logger> logger_;
    std::uint64_t nextSessionId_{0};
};

} // namespace svitock::infrastructure::network::websocket
