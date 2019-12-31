#include <iostream>
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
	char buf[32];   //EventLoop������     
	for (int i = 0; i < m_thread_num; i++) {
		snprintf(buf, 32, "Thread-->%d", i);
		std::shared_ptr<EventLoop> sp(new EventLoop);
		//��ʼ����EventLoop
		sp->init_with_name(buf);
		//�Ը�EventLoop::start_loop��������һ���߳�
		m_thread_vect.push_back(std::thread(&EventLoop::start_loop, &(*sp)));
		m_vect_event_loop.push_back(sp);
	}
}

std::shared_ptr<EventLoop> ThreadPool::select_one_thread_loop(void)
{
	//����ѡ�����߳�
	std::shared_ptr<EventLoop> selected = m_main_event_loop;
	//debug
	std::cout << "m_thread_num=" << m_thread_num << std::endl;
	if (m_thread_num > 0) {
		//˳��ѡȡ
		if (++m_position >= m_thread_num)
			m_position = 0;
		//debug
		std::cout << "m_position=" << m_position << std::endl;
		selected = m_vect_event_loop[m_position];
	}

	return selected;
}
