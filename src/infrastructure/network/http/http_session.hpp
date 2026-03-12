#pragma once

#include <memory>

#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http.hpp>

#include "infrastructure/network/http/http_router.hpp"

namespace svitock::infrastructure::network::http {

class HttpSession final : public std::enable_shared_from_this<HttpSession> {
public:
    HttpSession(boost::asio::ip::tcp::socket socket, HttpRouter& router);

    void run();

private:
    void doRead();
    void onRead(boost::beast::error_code ec, std::size_t bytesTransferred);
    void onWrite(boost::beast::error_code ec, std::size_t bytesTransferred, bool close);

    boost::beast::tcp_stream stream_;
    boost::beast::flat_buffer buffer_;
    boost::beast::http::request<boost::beast::http::string_body> request_;
    HttpRouter& router_;
};

} // namespace svitock::infrastructure::network::http
