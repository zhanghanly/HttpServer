#include <iostream>
#include <future>      //for std::async
#include <errno.h>
#include "EpollDispatcher.h"

EpollDispatcher::~EpollDispatcher(void)
{
	close(m_epoll_fd);
}

void EpollDispatcher::init(void)
{
	m_epoll_fd = epoll_create1(0);
	if (m_epoll_fd < 0)
		std::cerr << "epoll_create1 error  " << strerror(errno) << std::endl;
}

void EpollDispatcher::add(const std::shared_ptr<Channel>& channel)
{
	int fd = channel->m_fd;
	int events = 0;
	if (channel->m_events == EventType::read) 
		events = events | EPOLLIN;
	
	if (channel->m_events == EventType::write)
		events = events | EPOLLOUT;
	
	struct epoll_event event;
	event.data.fd = fd;
	event.events = events;
	//event.events = events | EPOLLET;  默认条件触发
	if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
		std::cerr << "epoll_ctl add  fd failed " << strerror(errno) << std::endl;
	
	m_fd_to_channel[fd] = channel;
}

void EpollDispatcher::remove(const std::shared_ptr<Channel>& channel)
{
	int fd = channel->m_fd;
	int events = 0;
	if (channel->m_events == EventType::read)
		events = events | EPOLLIN;
	
	if (channel->m_events == EventType::write) 
		events = events | EPOLLOUT;
	
	struct epoll_event event;
	event.data.fd = fd;
	event.events = events;
	//event.events = events | EPOLLET;
	if (epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, &event) == -1)
		std::cerr << "epoll_ctl delete fd failed " << strerror(errno) << std::endl;

	//关闭该套接字   如果先close再删除则会报错
	close(fd);

	m_fd_to_channel.erase(fd);
}

void EpollDispatcher::update(const std::shared_ptr<Channel>& channel)
{
	int fd = channel->m_fd;
	int events = 0;
	if (channel->m_events == EventType::read)
		events = events | EPOLLIN;

	if (channel->m_events == EventType::write)
		events = events | EPOLLOUT;

	struct epoll_event event;
	event.data.fd = fd;
	event.events = events;
	//event.events = events | EPOLLET;
	if (epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, fd, &event) == -1)
		std::cerr << "epoll_ctl modify fd failed " << strerror(errno) << std::endl;

	m_fd_to_channel[fd] = channel;
}

void EpollDispatcher::dispatch(void)
{
	//不设超时
	int n = epoll_wait(m_epoll_fd, &m_ready_epoll_event[0], epoll_ready_event_num, -1);
	//yolanda_msgx("epoll_wait wakeup, %s", eventLoop->thread_name);
	for (int i = 0; i < n; i++) {
		if ((m_ready_epoll_event[i].events & EPOLLERR) || (m_ready_epoll_event[i].events & EPOLLHUP)) {
			std::cerr << "epoll error, close fd" << std::endl;
			close(m_ready_epoll_event[i].data.fd);
			continue;
		}

		if (m_ready_epoll_event[i].events & EPOLLIN) {
			std::cout << "get message channel fd==" << m_ready_epoll_event[i].data.fd << std::endl;
			std::shared_ptr<Channel> channel = m_fd_to_channel[m_ready_epoll_event[i].data.fd];
			//让回调在一个线程中去执行
			//std::async(std::launch::async, channel->m_read_callback, channel->m_data);
			//执行读回调
			channel->m_read_callback(channel->m_data);
			//如果写回调是可以的, 就执行
			if (channel->write_event_is_enabled())
				channel->m_write_callback(channel->m_data);
		}

		if (m_ready_epoll_event[i].events & EPOLLOUT) {
			//std::cout << "get message channel fd==" << m_ready_epoll_event[i].data.fd << std::endl;
			//std::shared_ptr<Channel> channel = m_fd_to_channel[m_ready_epoll_event[i].data.fd];
			//std::async(std::launch::async, channel->m_write_callback, channel->m_data);
		}
	}
}

void EpollDispatcher::clear(void)
{

}