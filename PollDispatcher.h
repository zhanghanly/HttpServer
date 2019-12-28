#ifndef POLL_DISPATCHER_H
#define POLL_DISPATCHER_H

#include "Dispatcher.h"

class PollDispatcher : public Dispatcher
{
public:
	virtual void init(void) override;
	virtual void add(const std::shared_ptr<Channel>&) override;
	virtual void remove(const std::shared_ptr<Channel>&) override;
	virtual void update(const std::shared_ptr<Channel>&) override;
	virtual void dispatch(void) override;
	virtual void clear(void) override;
};

#endif