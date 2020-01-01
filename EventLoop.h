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
#include "SelectDispatcher.h"

class EventLoop
{
public:
	EventLoop(void) : m_dispatcher(new SelectDispatcher) {}
	void init_with_name(const std::string&);
	void start_loop(void);
	void add_channel_event(std::shared_ptr<Channel>&);
	void update_channel_event(std::shared_ptr<Channel>&);
	void remove_channel_event(std::shared_ptr<Channel>&);
	//����==�����, �����ж�����EventLoop�Ƿ����
	bool operator==(const EventLoop&);
	//�Ƿ���ͬ����һ���߳�
	void is_write_sockpair(void);
	//sockpair�Ķ��˴�����
	static int handle_wakeup(void *data);

private:
	void handle_pending_channel(void);

private:
	std::shared_ptr<Dispatcher> m_dispatcher;
	std::mutex m_mut;
	std::list<std::shared_ptr<Channel>> m_channel_lst;
	std::thread::id m_thread_id;      //������loop�̵߳��߳�ID
	std::string m_name;
	int m_socket_pair[2];
};

#endif
