#pragma once

#include <memory>

#include <boost/asio/io_context.hpp>
#include <spdlog/logger.h>

#include "application/services/realtime_dispatcher.hpp"

namespace svitock::infrastructure::websocket {

class WebSocketServer final : public application::services::RealtimeDispatcher {
public:
    WebSocketServer(boost::asio::io_context& ioContext,
                    std::uint16_t port,
                    std::shared_ptr<spdlog::logger> logger) noexcept;

    void run();
    void publishToChat(const boost::uuids::uuid& chatId, const std::string& payload) override;

private:
    boost::asio::io_context& ioContext_;
    std::uint16_t port_;
    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace svitock::infrastructure::websocket
