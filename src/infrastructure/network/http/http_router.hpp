#pragma once

#include <memory>
#include <string>

#include <boost/beast/http.hpp>

#include "application/interfaces/message_repository.hpp"

namespace svitock::core::security {
class JwtService;
}

namespace svitock::infrastructure::network::http {

namespace beast_http = boost::beast::http;

class HttpRouter {
public:
    HttpRouter(application::interfaces::MessageRepository& messageRepository,
               core::security::JwtService& jwtService);

    beast_http::response<beast_http::string_body> route(
        const beast_http::request<beast_http::string_body>& request) const;

private:
    application::interfaces::MessageRepository& messageRepository_;
    core::security::JwtService& jwtService_;
};

} // namespace svitock::infrastructure::network::http
