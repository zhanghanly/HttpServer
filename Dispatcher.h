#ifndef DISPATCHER_H
#define DISPATCHER_H

class Dispatcher
{
public:
	virtual void init(void) = 0;
	virtual void add(void) = 0;
	virtual void del(void) = 0;
	virtual void update(void) = 0;
	virtual void dispatch(void) = 0;
	virtual void clear(void) = 0;
};

#endif