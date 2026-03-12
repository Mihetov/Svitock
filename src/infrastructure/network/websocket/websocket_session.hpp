#pragma once

#include <deque>
#include <memory>
#include <string>

#include <boost/asio/strand.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/websocket.hpp>
#include <spdlog/logger.h>

namespace svitock::infrastructure::dispatcher {
class MessageDispatcher;
}

namespace svitock::infrastructure::actors {
class SessionActor;
}

namespace svitock::infrastructure::network::websocket {

class SessionTransport {
public:
    virtual ~SessionTransport() = default;
    virtual void send(std::string payload) = 0;
};

class WebSocketSession final : public SessionTransport,
                               public std::enable_shared_from_this<WebSocketSession> {
public:
    using TcpSocket = boost::asio::ip::tcp::socket;

    WebSocketSession(TcpSocket socket,
                     std::string sessionId,
                     dispatcher::MessageDispatcher& dispatcher,
                     actors::SessionActor& sessionActor,
                     std::shared_ptr<spdlog::logger> logger);

    void run();
    void send(std::string payload) override;

private:
    void onAccept(boost::beast::error_code ec);
    void doRead();
    void onRead(boost::beast::error_code ec, std::size_t bytesTransferred);
    void doWrite();
    void onWrite(boost::beast::error_code ec, std::size_t bytesTransferred);

    boost::beast::websocket::stream<boost::beast::tcp_stream> ws_;
    boost::beast::flat_buffer readBuffer_;
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;

    std::string sessionId_;
    dispatcher::MessageDispatcher& dispatcher_;
    actors::SessionActor& sessionActor_;
    std::shared_ptr<spdlog::logger> logger_;
    std::deque<std::string> writeQueue_;
};

} // namespace svitock::infrastructure::network::websocket
