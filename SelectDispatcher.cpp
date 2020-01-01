#include <iostream>
#include "SelectDispatcher.h"
#include "Common.h"

void SelectDispatcher::init(void)
{
	FD_ZERO(&m_read_set);
	FD_ZERO(&m_write_set);
}

void SelectDispatcher::add(const std::shared_ptr<Channel>& channel)
{
	int fd = channel->m_fd;

	if (channel->m_events == EventType::read)
		FD_SET(fd, &m_read_set);

	if (channel->m_events == EventType::write)
		FD_SET(fd, &m_write_set);

	m_fd_to_channel[fd] = channel;
}

void SelectDispatcher::remove(const std::shared_ptr<Channel>& channel)
{
	int fd = channel->m_fd;
	
	if (channel->m_events == EventType::read)
		FD_CLR(fd, &m_read_set);

	if (channel->m_events == EventType::write)
		FD_CLR(fd, &m_write_set);

	//关闭该套接字   如果先close再删除则会报错
	close(fd);
	//从map中删除
	m_fd_to_channel.erase(fd);
}

void SelectDispatcher::update(const std::shared_ptr<Channel>& channel)
{
	int fd = channel->m_fd;

	if (channel->m_events == EventType::read)
		FD_SET(fd, &m_read_set);

	if (channel->m_events == EventType::write)
		FD_SET(fd, &m_write_set);

	m_fd_to_channel[fd] = channel;
}

void SelectDispatcher::dispatch(void)
{
	fd_set read_set = m_read_set;
	fd_set write_set = m_write_set;

	auto iter = m_fd_to_channel.end();
	//最大的fd值
	int max_fd = (--iter)->first;
	//永远等下去
	int nfd = select(max_fd + 1, &read_set, &write_set, nullptr, nullptr);
	if (nfd < 0) {
		std::cerr << "select error" << std::endl;
		return;
	}
	for (iter = m_fd_to_channel.begin(); iter != m_fd_to_channel.end(); iter++) {
		if (FD_ISSET(iter->first, &read_set)) {
			std::cout << "get message channel fd==" << iter->first << std::endl;
			std::shared_ptr<Channel> channel = m_fd_to_channel[iter->first];
			//执行读回调
			channel->m_read_callback(channel->m_data);
			//如果写回调是可以的, 就执行
			if (channel->write_event_is_enabled())
				channel->m_write_callback(channel->m_data);
			--nfd;
		}
		if (FD_ISSET(iter->first, &write_set)) {

		}
		if (nfd == 0)
			break;
	}
}

void SelectDispatcher::clear(void)
{

}