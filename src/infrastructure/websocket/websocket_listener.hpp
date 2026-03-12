#pragma once

#include <memory>

#include <boost/asio/ip/tcp.hpp>
#include <spdlog/logger.h>

namespace svitock::infrastructure::websocket {

class MessageDispatcher;
class SessionManager;

class WebSocketListener final : public std::enable_shared_from_this<WebSocketListener> {
public:
    WebSocketListener(boost::asio::io_context& ioContext,
                      const boost::asio::ip::tcp::endpoint& endpoint,
                      MessageDispatcher& dispatcher,
                      SessionManager& sessionManager,
                      std::shared_ptr<spdlog::logger> logger);

    void run();

private:
    void doAccept();
    void onAccept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);

    boost::asio::ip::tcp::acceptor acceptor_;
    MessageDispatcher& dispatcher_;
    SessionManager& sessionManager_;
    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace svitock::infrastructure::websocket
