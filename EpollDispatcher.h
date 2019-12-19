#ifndef EPOLL_DISPATCHER_H
#define EPOLL_DISPATCHER_H

#include "Dispatcher.h"

class EpollDispatcher : public Dispatcher
{
public:
	virtual void init(void) override;
	virtual void add(void) override;
	virtual void del(void) override;
	virtual void update(void) override;
	virtual void dispatch(void) override;
	virtual void clear(void) override;
};

#endif
