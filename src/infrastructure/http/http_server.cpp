#include "infrastructure/http/http_server.hpp"

namespace svitock::infrastructure::http {

HttpServer::HttpServer(boost::asio::io_context& ioContext,
                       const std::uint16_t port,
                       std::shared_ptr<spdlog::logger> logger) noexcept
    : ioContext_(ioContext), port_(port), logger_(std::move(logger)) {}

void HttpServer::run() {
    logger_->info("HTTP server initialized on port {}", port_);
    (void)ioContext_;
}

} // namespace svitock::infrastructure::http
