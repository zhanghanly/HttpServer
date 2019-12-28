#include "EventLoop.h"

void EventLoop::init_with_name(const std::string& name)
{
	
	m_dispatcher->init();
}

void EventLoop::start_loop(void)
{
	while (true) {
		m_dispatcher->dispatch();
		handle_pending_channel();
	}
}

void EventLoop::add_channel_event(std::shared_ptr<Channel>& channel)
{
	channel->m_channel_type = ChannelType::add;
	std::lock_guard<std::mutex> lg(m_mut);
	m_channel_lst.push_back(channel);
}

void EventLoop::update_channel_event(std::shared_ptr<Channel>& channel)
{
	channel->m_channel_type = ChannelType::update;
	std::lock_guard<std::mutex> lg(m_mut);
	m_channel_lst.push_back(channel);
}

void EventLoop::remove_channel_event(std::shared_ptr<Channel>& channel)
{
	channel->m_channel_type = ChannelType::remove;
	std::lock_guard<std::mutex> lg(m_mut);
	m_channel_lst.push_back(channel);
}

void EventLoop::handle_pending_channel(void)
{
	std::lock_guard<std::mutex> lg(m_mut);
	while (!m_channel_lst.empty()) {
		std::shared_ptr<Channel> channel = m_channel_lst.front();
		switch (channel->m_channel_type){
		case ChannelType::add:
			m_dispatcher->add(channel);
			break;
		case ChannelType::remove:
			m_dispatcher->remove(channel);
			break;
		case ChannelType::update:
			m_dispatcher->update(channel);
			break;
		}
		//删除已经处理过的channel
		m_channel_lst.pop_front();
	}
}

 