#pragma once

#include "YTL/signal/cAsyncSignal.h"
#include "YTL/container/cStack.h"
#include "YTL/mpl/cAccessBase.h"
#include "System/thread/cThread.h"
#include <chrono>

namespace yame
{
namespace ytl
{

template<typename Message>
class message_producer
{
public:
    message_producer();
    virtual ~message_producer() = default;
    template<typename ... Args>
    inline void push(Args ... i_args);
    inline void enable(bool i_enable);
    inline bool is_enabled() const;

protected:
    container::cStack<Message> m_queue;
    std::atomic<bool>   m_enabled;
};

template<typename Message, typename KeyProducer,typename KeyConsumer>
class message_queue : protected message_producer<Message>
{
    YTL_PUBLISH_ACCESS(KeyProducer,message_queue,message_producer<Message>)

public:
    using message_producer<Message>::enable;
    using message_producer<Message>::is_enabled;

    message_queue(async_signal<void(const Message&),KeyConsumer>& m_signal, time_t i_updatePeriod = 100);
    message_queue(const message_queue&) = delete;
    ~message_queue();

    inline void start();
    inline void stop();
    inline bool is_stoppable() const;

private:
    void update();

    async_signal<void(const Message&),KeyConsumer>& m_signal;
    system::thread      m_updateThread;
    std::chrono::duration<double, std::milli>   m_updatePeriod;
};

}
}

#include "YTL/signal/detail/cMessageQueue.tpp"
