#pragma once

#include <memory>

#include <boost/asio/io_context.hpp>
#include <spdlog/logger.h>

namespace svitock::infrastructure::http {

class HttpServer {
public:
    HttpServer(boost::asio::io_context& ioContext,
               std::uint16_t port,
               std::shared_ptr<spdlog::logger> logger) noexcept;

    void run();

private:
    boost::asio::io_context& ioContext_;
    std::uint16_t port_;
    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace svitock::infrastructure::http
