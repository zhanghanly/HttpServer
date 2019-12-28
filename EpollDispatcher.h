#ifndef EPOLL_DISPATCHER_H
#define EPOLL_DISPATCHER_H

#include <memory>
#include <map>
#include "Common.h"
#include "Dispatcher.h"

constexpr int epoll_ready_event_num = 128;

class EpollDispatcher : public Dispatcher
{
public:
	EpollDispatcher(void) : m_ready_epoll_event(new epoll_event[epoll_ready_event_num]) {}
	~EpollDispatcher(void);
	virtual void init(void) override;
	virtual void add(const std::shared_ptr<Channel>&) override;
	virtual void remove(const std::shared_ptr<Channel>&) override;
	virtual void update(const std::shared_ptr<Channel>&) override;
	virtual void dispatch(void) override;
	virtual void clear(void) override;

private:
	int m_epoll_fd;     //epoll的句柄
	std::unique_ptr<epoll_event[]> m_ready_epoll_event;			//准备好的epoll事件
	std::map<int, std::shared_ptr<Channel>> m_fd_to_channel;    //fd到Channel得映射
};

#endif
