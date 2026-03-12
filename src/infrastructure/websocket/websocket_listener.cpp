#include "infrastructure/websocket/websocket_listener.hpp"

#include <boost/beast/core/error.hpp>

#include "infrastructure/websocket/message_dispatcher.hpp"
#include "infrastructure/websocket/session_manager.hpp"
#include "infrastructure/websocket/websocket_session.hpp"

namespace svitock::infrastructure::websocket {

WebSocketListener::WebSocketListener(boost::asio::io_context& ioContext,
                                     const boost::asio::ip::tcp::endpoint& endpoint,
                                     MessageDispatcher& dispatcher,
                                     SessionManager& sessionManager,
                                     std::shared_ptr<spdlog::logger> logger)
    : acceptor_(ioContext), dispatcher_(dispatcher), sessionManager_(sessionManager), logger_(std::move(logger)) {
    boost::beast::error_code ec;

    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
        throw std::runtime_error("acceptor open failed: " + ec.message());
    }

    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec) {
        throw std::runtime_error("acceptor set_option failed: " + ec.message());
    }

    acceptor_.bind(endpoint, ec);
    if (ec) {
        throw std::runtime_error("acceptor bind failed: " + ec.message());
    }

    acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
    if (ec) {
        throw std::runtime_error("acceptor listen failed: " + ec.message());
    }
}

void WebSocketListener::run() {
    logger_->info("websocket listener started at {}:{}",
                  acceptor_.local_endpoint().address().to_string(),
                  acceptor_.local_endpoint().port());
    doAccept();
}

void WebSocketListener::doAccept() {
    acceptor_.async_accept([self = shared_from_this()](boost::beast::error_code ec, boost::asio::ip::tcp::socket socket) {
        self->onAccept(ec, std::move(socket));
    });
}

void WebSocketListener::onAccept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket) {
    if (ec) {
        logger_->error("accept failed: {}", ec.message());
    } else {
        auto session = std::make_shared<WebSocketSession>(
            std::move(socket), dispatcher_, sessionManager_, logger_);
        session->run();
    }
    doAccept();
}

} // namespace svitock::infrastructure::websocket
