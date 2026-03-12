#pragma once

#include <memory>

#include <boost/asio/ip/tcp.hpp>

#include "infrastructure/network/http/http_router.hpp"

namespace svitock::infrastructure::network::http {

class HttpListener final : public std::enable_shared_from_this<HttpListener> {
public:
    HttpListener(boost::asio::io_context& ioContext,
                 const boost::asio::ip::tcp::endpoint& endpoint,
                 HttpRouter& router);

    void run();

private:
    void doAccept();
    void onAccept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);

    boost::asio::ip::tcp::acceptor acceptor_;
    HttpRouter& router_;
};

} // namespace svitock::infrastructure::network::http
