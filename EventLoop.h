#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <memory>
#include <list>
#include <mutex>
#include <map>
#include <string>
#include "Dispatcher.h"
#include "Channel.h"
#include "EpollDispatcher.h"

class EventLoop
{
public:
	EventLoop(void) : m_dispatcher(new EpollDispatcher) {}
	void init_with_name(const std::string&);
	void start_loop(void);
	void add_channel_event(std::shared_ptr<Channel>&);
	void update_channel_event(std::shared_ptr<Channel>&);
	void remove_channel_event(std::shared_ptr<Channel>&);

private:
	void handle_pending_channel(void);
	/*void pending_channel_add(void);
	void pending_channel_remove(void);
	void pending_channel_update(void);*/

private:
	std::shared_ptr<Dispatcher> m_dispatcher;
	std::mutex m_mut;
	std::list<std::shared_ptr<Channel>> m_channel_lst;
};

#endif
