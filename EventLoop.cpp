#include <iostream>
#include "EventLoop.h"

bool EventLoop::operator==(const EventLoop& el)
{
	//string判等是按照实际的字符串内容实现的
	return m_name == el.m_name;
}

void EventLoop::is_write_sockpair(void)
{
	if (m_thread_id != std::this_thread::get_id()) {
		char one = 'a';
		ssize_t n = write(m_socket_pair[0], &one, sizeof one);
		if (n != sizeof(one)) {
			std::cerr << "wakeup event loop thread failed" << std::endl;
		}
	}
}

void EventLoop::init_with_name(const std::string& name)
{
	m_name = name;

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, m_socket_pair) < 0)
		std::cerr << "socketpair set fialed" << std::endl;

	std::shared_ptr<Channel> channel(new Channel(m_socket_pair[1], EventType::read, this, 
									 &EventLoop::handle_wakeup, &EventLoop::handle_wakeup));
	add_channel_event(channel);

	m_dispatcher->init();
}

void EventLoop::start_loop(void)
{
	//获取本线程的ID
	m_thread_id = std::this_thread::get_id();
	while (true) {
		handle_pending_channel();
		m_dispatcher->dispatch();
	}
}

void EventLoop::add_channel_event(std::shared_ptr<Channel>& channel)
{
	channel->m_channel_type = ChannelType::add;
	std::lock_guard<std::mutex> lg(m_mut);
	m_channel_lst.push_back(channel);
	is_write_sockpair();
}

void EventLoop::update_channel_event(std::shared_ptr<Channel>& channel)
{
	channel->m_channel_type = ChannelType::update;
	std::lock_guard<std::mutex> lg(m_mut);
	m_channel_lst.push_back(channel);
	is_write_sockpair();
}

void EventLoop::remove_channel_event(std::shared_ptr<Channel>& channel)
{
	channel->m_channel_type = ChannelType::remove;
	std::lock_guard<std::mutex> lg(m_mut);
	m_channel_lst.push_back(channel);
	is_write_sockpair();
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

int EventLoop::handle_wakeup(void *data) 
{
	EventLoop* event_loop = (EventLoop*)data;
	char one;
	ssize_t n = read(event_loop->m_socket_pair[1], &one, sizeof(one));
	if (n != sizeof(one))
		std::cerr << "handleWakeup  failed" << std::endl;
	
	std::cout << "wakeup, " << event_loop->m_name << std::endl;
}

 