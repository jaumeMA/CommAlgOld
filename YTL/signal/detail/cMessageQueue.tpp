
namespace yame
{
namespace ytl
{

template<typename Message>
message_producer<Message>::message_producer()
{
    m_enabled.store(false);
}
template<typename Message>
template<typename ... Args>
void message_producer<Message>::push(Args ... i_args)
{
    if(m_enabled.load())
    {
        m_queue.push(mpl::forward<Args>(i_args) ...);
    }
}
template<typename Message>
void message_producer<Message>::enable(bool i_enable)
{
    bool isCurrentlyEnabled = m_enabled.load();

    if(isCurrentlyEnabled != i_enable)
    {
        m_enabled.compare_exchange_weak(isCurrentlyEnabled,i_enable);
    }
}
template<typename Message>
bool message_producer<Message>::is_enabled() const
{
    return m_enabled.load();
}

template<typename Message, typename KeyProducer, typename KeyConsumer>
message_queue<Message,KeyProducer,KeyConsumer>::message_queue(async_signal<void(const Message&),KeyConsumer>& i_signal, time_t i_updatePeriod)
: m_updatePeriod(i_updatePeriod)
, m_signal(i_signal)
{
}
template<typename Message, typename KeyProducer, typename KeyConsumer>
void message_queue<Message,KeyProducer,KeyConsumer>::start()
{
    m_updateThread.start(ytl::make_function(this,&message_queue<Message,KeyProducer,KeyConsumer>::update));

    this->enable(true);
}
template<typename Message, typename KeyProducer, typename KeyConsumer>
message_queue<Message,KeyProducer,KeyConsumer>::~message_queue()
{
    stop();
}
template<typename Message, typename KeyProducer, typename KeyConsumer>
void message_queue<Message,KeyProducer,KeyConsumer>::stop()
{
    this->enable(false);

    m_updateThread.stop();
}
template<typename Message, typename KeyProducer, typename KeyConsumer>
bool message_queue<Message,KeyProducer,KeyConsumer>::is_stoppable() const
{
    return m_updateThread.is_stoppable();
}
template<typename Message, typename KeyProducer, typename KeyConsumer>
void message_queue<Message,KeyProducer,KeyConsumer>::update()
{
    do
    {
        while(this->m_queue.empty() == false)
        {
            if(ytl::optional<Message> lastMsg = this->m_queue.pop())
            {
                access<KeyConsumer>(m_signal).execute(*lastMsg);
            }
        }

        std::this_thread::sleep_for(m_updatePeriod);
    }
    while(m_updateThread.is_stopped() == false);

    ASSERT(this->m_enabled.load() == false, "We are stopped but not disabled!");

    //at this point we are stopped and so not enabled, finish processing pending messages (producer cannot insert any other message)
    while(this->m_queue.empty() == false)
    {
        if(ytl::optional<Message> lastMsg = this->m_queue.pop())
        {
            access<KeyConsumer>(m_signal).execute(*lastMsg);
        }
    }
}

}
}
