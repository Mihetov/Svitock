cmake_minimum_required(VERSION 3.25)
project(svitock-backend VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Boost 1.79 REQUIRED COMPONENTS system)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)

add_library(svitock_domain STATIC
    src/domain/user/user.cpp
    src/domain/chat/chat.cpp
    src/domain/message/message.cpp
)
target_include_directories(svitock_domain PUBLIC src)

add_library(svitock_application STATIC
    src/application/use_cases/register_user_use_case.cpp
    src/application/use_cases/login_user_use_case.cpp
    src/application/use_cases/create_chat_use_case.cpp
    src/application/use_cases/send_message_use_case.cpp
    src/application/use_cases/get_chat_history_use_case.cpp
)
target_include_directories(svitock_application PUBLIC src)
target_link_libraries(svitock_application PUBLIC svitock_domain)

add_library(svitock_core STATIC
    src/core/config/app_config.cpp
    src/core/logging/logger.cpp
    src/core/threading/thread_pool.cpp
    src/core/security/jwt_service.cpp
    src/core/actor/mailbox.cpp
    src/core/actor/actor_system.cpp
)
target_include_directories(svitock_core PUBLIC src)
target_link_libraries(svitock_core PUBLIC yaml-cpp::yaml-cpp spdlog::spdlog nlohmann_json::nlohmann_json)

add_library(svitock_infrastructure STATIC
    src/infrastructure/database/postgres_pool.cpp
    src/infrastructure/repositories/postgres_user_repository.cpp
    src/infrastructure/repositories/postgres_chat_repository.cpp
    src/infrastructure/repositories/postgres_message_repository.cpp
    src/infrastructure/actors/session_actor.cpp
    src/infrastructure/actors/chat_actor.cpp
    src/infrastructure/actors/message_actor.cpp
    src/infrastructure/actors/storage_actor.cpp
    src/infrastructure/dispatcher/message_dispatcher.cpp
    src/infrastructure/network/http/http_router.cpp
    src/infrastructure/network/http/http_listener.cpp
    src/infrastructure/network/http/http_session.cpp
    src/infrastructure/network/websocket/websocket_listener.cpp
    src/infrastructure/network/websocket/websocket_session.cpp
    src/infrastructure/network/server.cpp
)
target_include_directories(svitock_infrastructure PUBLIC src)
target_include_directories(svitock_infrastructure SYSTEM PUBLIC ${LIBPQXX_INCLUDE_DIRS})
target_link_libraries(svitock_infrastructure
    PUBLIC
        svitock_application
        svitock_core
        Boost::system
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        PostgreSQL::PostgreSQL
        ${LIBPQXX_LIBRARIES}
)

add_executable(svitock_backend src/main.cpp)
target_link_libraries(svitock_backend PRIVATE svitock_infrastructure)

foreach(target svitock_domain svitock_application svitock_core svitock_infrastructure svitock_backend)
    target_compile_options(${target} PRIVATE -O3 -DNDEBUG -Wall -Wextra -Wpedantic)
endforeach()
