#pragma once

#include <cstddef>
#include <thread>
#include <vector>

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>

namespace svitock::core::threading {

class ThreadPool {
public:
    explicit ThreadPool(boost::asio::io_context& ioContext);

    void start(std::size_t threads);
    void stop();

private:
    boost::asio::io_context& ioContext_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> workGuard_;
    std::vector<std::thread> workers_;
};

} // namespace svitock::core::threading
