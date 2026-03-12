#include "core/threading/thread_pool.hpp"

namespace svitock::core::threading {

ThreadPool::ThreadPool(boost::asio::io_context& ioContext)
    : ioContext_(ioContext), workGuard_(boost::asio::make_work_guard(ioContext)) {}

void ThreadPool::start(const std::size_t threads) {
    workers_.reserve(threads);
    for (std::size_t i = 0; i < threads; ++i) {
        workers_.emplace_back([this] { ioContext_.run(); });
    }
}

void ThreadPool::stop() {
    workGuard_.reset();
    ioContext_.stop();
    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    workers_.clear();
}

} // namespace svitock::core::threading
