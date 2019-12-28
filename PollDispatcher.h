#ifndef POLL_DISPATCHER_H
#define POLL_DISPATCHER_H

#include "Dispatcher.h"

class PollDispatcher : public Dispatcher
{
public:
	virtual void init(void) override;
	virtual void add(void) override;
	virtual void remove(void) override;
	virtual void update(void) override;
	virtual void dispatch(void) override;
	virtual void clear(void) override;
};

#endif