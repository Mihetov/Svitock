#pragma once

#include <deque>
#include <memory>
#include <string>

#include <boost/asio/strand.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/websocket.hpp>
#include <spdlog/logger.h>

namespace svitock::infrastructure::websocket {

class MessageDispatcher;
class SessionManager;

class WebSocketSession final : public std::enable_shared_from_this<WebSocketSession> {
public:
    using TcpSocket = boost::asio::ip::tcp::socket;

    WebSocketSession(TcpSocket socket,
                     MessageDispatcher& dispatcher,
                     SessionManager& sessionManager,
                     std::shared_ptr<spdlog::logger> logger);

    void run();
    void sendText(std::string message);

    [[nodiscard]] const std::string& getUserId() const noexcept;
    void setUserId(std::string userId);

private:
    void onAccept(boost::beast::error_code ec);
    void doRead();
    void onRead(boost::beast::error_code ec, std::size_t bytesTransferred);

    void doWrite();
    void onWrite(boost::beast::error_code ec, std::size_t bytesTransferred);

    void close();

    boost::beast::websocket::stream<boost::beast::tcp_stream> ws_;
    boost::beast::flat_buffer readBuffer_;
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;

    MessageDispatcher& dispatcher_;
    SessionManager& sessionManager_;
    std::shared_ptr<spdlog::logger> logger_;

    std::deque<std::string> writeQueue_;
    std::string userId_;
};

} // namespace svitock::infrastructure::websocket
