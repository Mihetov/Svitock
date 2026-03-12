#include "infrastructure/network/http/http_session.hpp"

namespace svitock::infrastructure::network::http {

HttpSession::HttpSession(boost::asio::ip::tcp::socket socket, HttpRouter& router)
    : stream_(std::move(socket)), router_(router) {}

void HttpSession::run() {
    doRead();
}

void HttpSession::doRead() {
    request_ = {};
    boost::beast::http::async_read(
        stream_,
        buffer_,
        request_,
        [self = shared_from_this()](const boost::beast::error_code ec, const std::size_t bytesTransferred) {
            self->onRead(ec, bytesTransferred);
        });
}

void HttpSession::onRead(const boost::beast::error_code ec, const std::size_t) {
    if (ec == boost::beast::http::error::end_of_stream) {
        boost::beast::error_code ignore;
        stream_.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send, ignore);
        return;
    }

    if (ec) {
        return;
    }

    auto response = std::make_shared<boost::beast::http::response<boost::beast::http::string_body>>(router_.route(request_));
    const bool close = response->need_eof();
    boost::beast::http::async_write(
        stream_,
        *response,
        [self = shared_from_this(), response, close](const boost::beast::error_code writeError,
                                                     const std::size_t bytesTransferred) {
            self->onWrite(writeError, bytesTransferred, close);
        });
}

void HttpSession::onWrite(const boost::beast::error_code ec, const std::size_t, const bool close) {
    if (ec) {
        return;
    }

    if (close) {
        boost::beast::error_code ignore;
        stream_.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send, ignore);
        return;
    }

    doRead();
}

} // namespace svitock::infrastructure::network::http
