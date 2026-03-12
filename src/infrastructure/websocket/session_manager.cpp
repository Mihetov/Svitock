#include "infrastructure/websocket/session_manager.hpp"

#include "infrastructure/websocket/websocket_session.hpp"

namespace svitock::infrastructure::websocket {

void SessionManager::registerSession(const std::string& userId, const std::shared_ptr<WebSocketSession>& session) {
    std::unique_lock lock(mutex_);
    sessions_[userId] = session;
}

void SessionManager::removeSession(const std::string& userId) {
    std::unique_lock lock(mutex_);
    sessions_.erase(userId);
}

std::shared_ptr<WebSocketSession> SessionManager::findSession(const std::string& userId) const {
    std::shared_lock lock(mutex_);
    const auto it = sessions_.find(userId);
    if (it == sessions_.end()) {
        return nullptr;
    }
    return it->second.lock();
}

} // namespace svitock::infrastructure::websocket
