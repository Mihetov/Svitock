#include "infrastructure/network/websocket/websocket_session.hpp"

#include <boost/asio/post.hpp>
#include <boost/beast/core/buffers_to_string.hpp>

#include "infrastructure/actors/session_actor.hpp"
#include "infrastructure/dispatcher/message_dispatcher.hpp"

namespace svitock::infrastructure::network::websocket {

WebSocketSession::WebSocketSession(TcpSocket socket,
                                   std::string sessionId,
                                   dispatcher::MessageDispatcher& dispatcher,
                                   actors::SessionActor& sessionActor,
                                   std::shared_ptr<spdlog::logger> logger)
    : ws_(std::move(socket)),
      strand_(ws_.get_executor()),
      sessionId_(std::move(sessionId)),
      dispatcher_(dispatcher),
      sessionActor_(sessionActor),
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
        logger_->error("ws accept error: {}", ec.message());
        return;
    }

    sessionActor_.bindTransport(sessionId_, weak_from_this());
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

void WebSocketSession::onRead(const boost::beast::error_code ec, const std::size_t) {
    if (ec) {
        sessionActor_.unbindTransport(sessionId_);
        return;
    }

    const auto raw = boost::beast::buffers_to_string(readBuffer_.data());
    readBuffer_.consume(readBuffer_.size());
    dispatcher_.dispatch(sessionId_, raw);
    doRead();
}

void WebSocketSession::send(std::string payload) {
    boost::asio::post(strand_, [self = shared_from_this(), payload = std::move(payload)]() mutable {
        const auto writing = !self->writeQueue_.empty();
        self->writeQueue_.push_back(std::move(payload));
        if (!writing) {
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

void WebSocketSession::onWrite(const boost::beast::error_code ec, const std::size_t) {
    if (ec) {
        sessionActor_.unbindTransport(sessionId_);
        return;
    }

    writeQueue_.pop_front();
    if (!writeQueue_.empty()) {
        doWrite();
    }
}

} // namespace svitock::infrastructure::network::websocket
