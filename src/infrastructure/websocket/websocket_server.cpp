#include "infrastructure/websocket/websocket_server.hpp"

#include <boost/uuid/uuid_io.hpp>

namespace svitock::infrastructure::websocket {

WebSocketServer::WebSocketServer(boost::asio::io_context& ioContext,
                                 const std::uint16_t port,
                                 std::shared_ptr<spdlog::logger> logger) noexcept
    : ioContext_(ioContext), port_(port), logger_(std::move(logger)) {}

void WebSocketServer::run() {
    logger_->info("WebSocket server initialized on port {}", port_);
    (void)ioContext_;
}

void WebSocketServer::publishToChat(const boost::uuids::uuid& chatId, const std::string& payload) {
    logger_->debug("Dispatching event to chat {} payload size {}", boost::uuids::to_string(chatId), payload.size());
}

} // namespace svitock::infrastructure::websocket
