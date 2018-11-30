#pragma once

#include "YTL/functional/cFunction.h"
#include <thread>
#include <atomic>

namespace yame
{
namespace system
{

class thread
{
public:
    thread();
    void start(const ytl::function<void()>& i_callable);
    void stop();
    bool is_stopped() const;
    bool is_stoppable() const;

private:
    std::atomic<bool>   m_stopped;
    std::thread         m_thread;
};

}
}
