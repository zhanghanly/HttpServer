#include "ThreadPool.h"

void ThreadPool::init_thread_pool(std::shared_ptr<EventLoop> event_loop, int thread_num)
{
	m_thread_num = thread_num;
	if (thread_num > 0) {
		m_vect_event_loop.reserve(thread_num);
		m_thread_vect.reserve(thread_num);
	}
	m_main_event_loop = event_loop;
}

void ThreadPool::start_thread_pool(void)
{
	for (int i = 0; i < m_thread_num; i++) {
		//std::shared_ptr<EventLoop> sp(new EventLoop);
		m_vect_event_loop.emplace_back(new EventLoop);
		m_vect_event_loop[i]->init_with_name("Thread");
		m_thread_vect[i] = std::thread(&EventLoop::start_loop, &(*m_vect_event_loop[i]));
	}
}

std::shared_ptr<EventLoop> ThreadPool::select_one_thread_loop(void)
{
	//优先选择主线程
	std::shared_ptr<EventLoop> selected = m_main_event_loop;
	if (m_thread_num > 0) {
		//顺序选取
		if (++m_position >= m_thread_num)
			m_position = -1;
		selected = m_vect_event_loop[m_position];
	}

	return selected;
}
