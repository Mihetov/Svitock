#include "infrastructure/network/http/http_listener.hpp"

#include "infrastructure/network/http/http_session.hpp"

namespace svitock::infrastructure::network::http {

HttpListener::HttpListener(boost::asio::io_context& ioContext,
                           const boost::asio::ip::tcp::endpoint& endpoint,
                           HttpRouter& router)
    : acceptor_(ioContext), router_(router) {
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

void HttpListener::run() {
    doAccept();
}

void HttpListener::doAccept() {
    acceptor_.async_accept([self = shared_from_this()](boost::beast::error_code ec, boost::asio::ip::tcp::socket socket) {
        self->onAccept(ec, std::move(socket));
    });
}

void HttpListener::onAccept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket) {
    if (!ec) {
        std::make_shared<HttpSession>(std::move(socket), router_)->run();
    }
    doAccept();
}

} // namespace svitock::infrastructure::network::http
