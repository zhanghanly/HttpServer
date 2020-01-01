#ifndef SELECT_DISPATCHER_H
#define SELECT_DISPATCHER_H

#include "Dispatcher.h"

class SelectDispatcher : public Dispatcher
{
public:
	virtual void init(void) override;
	virtual void add(const std::shared_ptr<Channel>&) override;
	virtual void remove(const std::shared_ptr<Channel>&) override;
	virtual void update(const std::shared_ptr<Channel>&) override;
	virtual void dispatch(void) override;
	virtual void clear(void) override;

private:
	std::map<int, std::shared_ptr<Channel>> m_fd_to_channel;  //fd到Channel的映射
	fd_set m_read_set;     //读事件集合
	fd_set m_write_set;    //写事件集合
};

#endif