#include "infrastructure/network/websocket/websocket_listener.hpp"

#include <boost/beast/core/error.hpp>

#include "infrastructure/actors/session_actor.hpp"
#include "infrastructure/dispatcher/message_dispatcher.hpp"
#include "infrastructure/network/websocket/websocket_session.hpp"

namespace svitock::infrastructure::network::websocket {

WebSocketListener::WebSocketListener(boost::asio::io_context& ioContext,
                                     boost::asio::ip::tcp::endpoint endpoint,
                                     dispatcher::MessageDispatcher& dispatcher,
                                     actors::SessionActor& sessionActor,
                                     std::shared_ptr<spdlog::logger> logger)
    : acceptor_(ioContext), dispatcher_(dispatcher), sessionActor_(sessionActor), logger_(std::move(logger)) {
    boost::beast::error_code ec;
    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
        throw std::runtime_error(ec.message());
    }
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec) {
        throw std::runtime_error(ec.message());
    }
    acceptor_.bind(endpoint, ec);
    if (ec) {
        throw std::runtime_error(ec.message());
    }
    acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
    if (ec) {
        throw std::runtime_error(ec.message());
    }
}

void WebSocketListener::run() {
    doAccept();
}

void WebSocketListener::doAccept() {
    acceptor_.async_accept([self = shared_from_this()](boost::beast::error_code ec, boost::asio::ip::tcp::socket socket) {
        self->onAccept(ec, std::move(socket));
    });
}

void WebSocketListener::onAccept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket) {
    if (!ec) {
        auto session = std::make_shared<WebSocketSession>(
            std::move(socket),
            std::to_string(++nextSessionId_),
            dispatcher_,
            sessionActor_,
            logger_);
        session->run();
    }
    doAccept();
}

} // namespace svitock::infrastructure::network::websocket
