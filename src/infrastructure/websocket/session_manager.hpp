#pragma once

#include <memory>
#include <shared_mutex>
#include <string>
#include <unordered_map>

namespace svitock::infrastructure::websocket {

class WebSocketSession;

class SessionManager {
public:
    void registerSession(const std::string& userId, const std::shared_ptr<WebSocketSession>& session);
    void removeSession(const std::string& userId);
    std::shared_ptr<WebSocketSession> findSession(const std::string& userId) const;

private:
    mutable std::shared_mutex mutex_;
    std::unordered_map<std::string, std::weak_ptr<WebSocketSession>> sessions_;
};

} // namespace svitock::infrastructure::websocket
