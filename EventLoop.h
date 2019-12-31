#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <memory>
#include <list>
#include <thread>
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
	//重载==运算符, 用来判断两个EventLoop是否相等
	bool operator==(const EventLoop&);
	//是否是同属于一个线程
	void is_write_sockpair(void);
	//sockpair的读端处理函数
	static int handle_wakeup(void *data);

private:
	void handle_pending_channel(void);

private:
	std::shared_ptr<Dispatcher> m_dispatcher;
	std::mutex m_mut;
	std::list<std::shared_ptr<Channel>> m_channel_lst;
	std::thread::id m_thread_id;      //启动该loop线程的线程ID
	std::string m_name;
	int m_socket_pair[2];
};

#endif
