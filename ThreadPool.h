#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "EventLoop.h"

class ThreadPool
{
public:
	ThreadPool(void) : m_thread_num(0), m_position(-1){}
	void init_thread_pool(std::shared_ptr<EventLoop>, int);
	void start_thread_pool(void);
	std::shared_ptr<EventLoop> select_one_thread_loop(void);

private:
	int m_thread_num;						//�̳߳��е��߳�����
	int m_position;                         //��ѡȡ�̵߳�λ��
	std::shared_ptr<EventLoop> m_main_event_loop;
	std::vector<std::shared_ptr<EventLoop>> m_vect_event_loop;
	std::vector<std::thread> m_thread_vect;
};

#endif
