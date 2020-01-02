#ifndef POLL_DISPATCHER_H
#define POLL_DISPATCHER_H

#include <vector>
#include "Dispatcher.h"
#include "Common.h"

class PollDispatcher : public Dispatcher
{
public:
	virtual void init(void) override;
	virtual void add(const std::shared_ptr<Channel>&) override;
	virtual void remove(const std::shared_ptr<Channel>&) override;
	virtual void update(const std::shared_ptr<Channel>&) override;
	virtual void dispatch(void) override;
	virtual void clear(void) override;

private:
	std::map<int, std::shared_ptr<Channel>> m_fd_to_channel;
	std::vector<pollfd> m_event_set;
};

#endif