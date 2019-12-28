#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <map>
#include "Channel.h"

class Dispatcher
{
public:
	virtual void init(void) = 0;
	virtual void add(const std::shared_ptr<Channel>&) = 0;
	virtual void remove(const std::shared_ptr<Channel>&) = 0;
	virtual void update(const std::shared_ptr<Channel>&) = 0;
	virtual void dispatch(void) = 0;
	virtual void clear(void) = 0;
	virtual ~Dispatcher(void) {}
};

#endif