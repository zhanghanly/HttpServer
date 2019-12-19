#ifndef SELECT_DISPATCHER_H
#define SELECT_DISPATCHER_H

#include "Dispatcher.h"

class SelectDispatcher : public Dispatcher
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