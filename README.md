# Svitock Backend

Production-ready backend skeleton for Svitock messenger with C++20, Clean + Hexagonal architecture, actor model core, and async WebSocket/HTTP networking.

## Layers

- `core`: config, logging, jwt, actor runtime, thread pools.
- `domain`: pure entities (`User`, `Chat`, `Message`).
- `application`: repository ports + use-cases.
- `infrastructure`: Postgres adapters, actor implementations, dispatcher, networking servers.
- `presentation`: DTO/controllers extension points.

Dependency rules:

- `presentation -> application -> domain`
- `infrastructure -> application`
- `core` shared by all
- `domain` has no outward dependencies

## Actor System

Implemented actors:

- `SessionActor`
- `ChatActor`
- `MessageActor`
- `StorageActor`

Design highlights:

- lock-free mailboxes (`boost::lockfree::queue`)
- actor workers process mailbox events in batches across actor threads
- networking thread never executes business/storage logic directly
- all cross-component communication via `ActorEnvelope` message passing

## Networking

### WebSocket (Boost.Beast)

- async accept/read/write
- per-connection `WebSocketSession`
- events: `auth`, `send_message`, `typing`, `message_read`, `ping`
- integrates with actor dispatcher (`infrastructure/dispatcher/MessageDispatcher`)

### HTTP API (Boost.Beast)

Async endpoints:

- `POST /register`
- `POST /login`
- `POST /refresh`
- `GET /user/{id}`
- `GET /chat/{id}/messages`
- `POST /chat/{id}/message`

JSON serialization: `nlohmann/json`.

JWT service: `core/security/JwtService` (token issue/validate/extract).

## Storage

- PostgreSQL via `libpqxx`
- pooled connections (`PostgresPool`)
- repository adapters in `src/infrastructure/repositories`
- `StorageActor` persists messages asynchronously from actor pipeline

## Build

Requirements:

- CMake >= 3.25
- C++20 compiler
- Boost (system)
- nlohmann_json
- spdlog
- yaml-cpp
- PostgreSQL client
- libpqxx

```bash
cmake -S . -B build
cmake --build build -j
```

Run:

```bash
./build/svitock_backend
```
