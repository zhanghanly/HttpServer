#include <iostream>
#include "PollDispatcher.h"

void PollDispatcher::init(void)
{
	//Ԥ�ȷ���1024��"�ռ�"
	m_event_set.reserve(1024);
	pollfd event_fd;
	event_fd.fd = -1;
	event_fd.events = 0;
	for (int i = 0; i < 1024; i++)
		m_event_set.push_back(event_fd);
}

void PollDispatcher::add(const std::shared_ptr<Channel>& channel)
{
	int fd = channel->m_fd;
	pollfd event_fd;
	event_fd.fd = fd;

	if (channel->m_events == EventType::read)
		event_fd.events |= POLLRDNORM;

	if (channel->m_events == EventType::write)
		event_fd.events |= POLLWRNORM;
	//flag�����жϿ��Ը���ԭ����λ�û�����Ҫpush_back
	int flag = 1;
	for (auto iter = m_event_set.begin(); iter != m_event_set.end(); iter++) {
		if (iter->fd == -1) {
			*iter = event_fd;
			flag = 0;
			break;
		}
	}
	if (flag)
		m_event_set.push_back(event_fd);

	m_fd_to_channel[fd] = channel;
}

void PollDispatcher::remove(const std::shared_ptr<Channel>& channel)
{
	int fd = channel->m_fd;
	 
	for (auto iter = m_event_set.begin(); iter != m_event_set.end(); iter++) {
		if (iter->fd == fd) {
			iter->fd = -1;
			iter->events = 0;
			//�رո��׽���
			close(fd);
			//��map��ɾ��
			m_fd_to_channel.erase(fd);
		}
	}
}

void PollDispatcher::update(const std::shared_ptr<Channel>& channel)
{
	add(channel);
}

void PollDispatcher::dispatch(void)
{
	int ready_fd = poll(&m_event_set[0], m_event_set.size(), -1);
	if (ready_fd < 0) {
		std::cerr << "poll error" << std::endl;
		return;
	}
	for (auto iter = m_event_set.begin(); iter != m_event_set.end(); iter++) {
		//������ڻص���ȥ����
		if (iter->revents & POLLRDNORM) {
			std::cout << "get message channel fd==" << iter->fd << std::endl;
			std::shared_ptr<Channel> channel = m_fd_to_channel[iter->fd];
			//ִ�ж��ص�
			channel->m_read_callback(channel->m_data);
			//���д�ص��ǿ��Ե�, ��ִ��
			if (channel->write_event_is_enabled())
				channel->m_write_callback(channel->m_data);
			--ready_fd;
			//iter->revents = 0;
		}
		if (ready_fd == 0)
			break;
	}
}

void PollDispatcher::clear(void)
{

}