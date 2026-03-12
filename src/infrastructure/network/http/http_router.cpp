#include "infrastructure/network/http/http_router.hpp"

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <nlohmann/json.hpp>

#include "core/security/jwt_service.hpp"

namespace svitock::infrastructure::network::http {

HttpRouter::HttpRouter(application::interfaces::MessageRepository& messageRepository,
                       core::security::JwtService& jwtService)
    : messageRepository_(messageRepository), jwtService_(jwtService) {}

beast_http::response<beast_http::string_body> HttpRouter::route(
    const beast_http::request<beast_http::string_body>& request) const {
    beast_http::response<beast_http::string_body> response;
    response.set(beast_http::field::content_type, "application/json");

    if (request.method() == beast_http::verb::post && request.target() == "/register") {
        response.result(beast_http::status::ok);
        response.body() = nlohmann::json{{"status", "ok"}}.dump();
    } else if (request.method() == beast_http::verb::post && request.target() == "/login") {
        const auto body = nlohmann::json::parse(request.body(), nullptr, false);
        const auto userId = body.value("user_id", "");
        response.result(beast_http::status::ok);
        response.body() = nlohmann::json{{"access_token", jwtService_.issueToken(userId, 3600)}}.dump();
    } else if (request.method() == beast_http::verb::post && request.target() == "/refresh") {
        const std::string auth{request[beast_http::field::authorization]};
        const auto token = auth.substr(auth.find(' ') + 1);
        if (!jwtService_.validateToken(token)) {
            response.result(beast_http::status::unauthorized);
            response.body() = nlohmann::json{{"error", "invalid token"}}.dump();
        } else {
            nlohmann::json tokenResponse;
            tokenResponse["access_token"] = jwtService_.issueToken(jwtService_.extractUserId(token), 3600);
            response.result(beast_http::status::ok);
            response.body() = tokenResponse.dump();
        }
    } else if (request.method() == beast_http::verb::get && request.target().starts_with("/user/")) {
        const auto id = std::string(request.target().substr(6));
        response.result(beast_http::status::ok);
        response.body() = nlohmann::json{{"id", id}}.dump();
    } else if (request.method() == beast_http::verb::get && request.target().starts_with("/chat/") && request.target().ends_with("/messages")) {
        const auto full = std::string(request.target());
        const auto chatId = full.substr(6, full.size() - 6 - 9);
        const auto messages = messageRepository_.getByChat(boost::uuids::string_generator{}(chatId), 50);
        nlohmann::json out = nlohmann::json::array();
        for (const auto& message : messages) {
            out.push_back(nlohmann::json{{"id", boost::uuids::to_string(message.getId())},
                                         {"text", message.getContent()}});
        }
        response.result(beast_http::status::ok);
        response.body() = out.dump();
    } else if (request.method() == beast_http::verb::post && request.target().starts_with("/chat/") && request.target().ends_with("/message")) {
        response.result(beast_http::status::accepted);
        response.body() = nlohmann::json{{"status", "queued"}}.dump();
    } else {
        response.result(beast_http::status::not_found);
        response.body() = nlohmann::json{{"error", "not found"}}.dump();
    }

    response.prepare_payload();
    return response;
}

} // namespace svitock::infrastructure::network::http
