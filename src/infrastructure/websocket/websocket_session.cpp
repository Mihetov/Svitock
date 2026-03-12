#include "infrastructure/websocket/websocket_session.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/post.hpp>
#include <boost/beast/core/buffers_to_string.hpp>

#include "infrastructure/websocket/message_dispatcher.hpp"
#include "infrastructure/websocket/session_manager.hpp"

namespace svitock::infrastructure::websocket {

WebSocketSession::WebSocketSession(TcpSocket socket,
                                   MessageDispatcher& dispatcher,
                                   SessionManager& sessionManager,
                                   std::shared_ptr<spdlog::logger> logger)
    : ws_(std::move(socket)),
      strand_(ws_.get_executor()),
      dispatcher_(dispatcher),
      sessionManager_(sessionManager),
      logger_(std::move(logger)) {}

void WebSocketSession::run() {
    ws_.set_option(boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::server));
    ws_.text(true);
    ws_.async_accept(
        boost::asio::bind_executor(strand_, [self = shared_from_this()](const boost::beast::error_code ec) {
            self->onAccept(ec);
        }));
}

void WebSocketSession::onAccept(const boost::beast::error_code ec) {
    if (ec) {
        logger_->error("websocket accept error: {}", ec.message());
        return;
    }
    logger_->info("websocket connection established");
    doRead();
}

void WebSocketSession::doRead() {
    ws_.async_read(
        readBuffer_,
        boost::asio::bind_executor(strand_, [self = shared_from_this()](const boost::beast::error_code ec,
                                                                         const std::size_t bytesTransferred) {
            self->onRead(ec, bytesTransferred);
        }));
}

void WebSocketSession::onRead(const boost::beast::error_code ec, const std::size_t /*bytesTransferred*/) {
    if (ec == boost::beast::websocket::error::closed) {
        close();
        return;
    }
    if (ec) {
        logger_->warn("websocket read error: {}", ec.message());
        close();
        return;
    }

    const auto raw = boost::beast::buffers_to_string(readBuffer_.data());
    readBuffer_.consume(readBuffer_.size());

    dispatcher_.dispatchMessage(*this, raw);
    doRead();
}

void WebSocketSession::sendText(std::string message) {
    boost::asio::post(
        strand_,
        [self = shared_from_this(), payload = std::move(message)]() mutable {
            const bool isWriting = !self->writeQueue_.empty();
            self->writeQueue_.push_back(std::move(payload));
            if (!isWriting) {
                self->doWrite();
            }
        });
}

void WebSocketSession::doWrite() {
    ws_.async_write(
        boost::asio::buffer(writeQueue_.front()),
        boost::asio::bind_executor(strand_, [self = shared_from_this()](const boost::beast::error_code ec,
                                                                         const std::size_t bytesTransferred) {
            self->onWrite(ec, bytesTransferred);
        }));
}

void WebSocketSession::onWrite(const boost::beast::error_code ec, const std::size_t /*bytesTransferred*/) {
    if (ec) {
        logger_->warn("websocket write error: {}", ec.message());
        close();
        return;
    }

    writeQueue_.pop_front();
    if (!writeQueue_.empty()) {
        doWrite();
    }
}

const std::string& WebSocketSession::getUserId() const noexcept {
    return userId_;
}

void WebSocketSession::setUserId(std::string userId) {
    userId_ = std::move(userId);
}

void WebSocketSession::close() {
    if (!userId_.empty()) {
        sessionManager_.removeSession(userId_);
    }
    logger_->info("websocket session closed user_id={}", userId_);
}

} // namespace svitock::infrastructure::websocket
