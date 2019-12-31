#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <list>
#include <mutex>
#include <thread>
#include "TCPconnection.h"
#include "Common.h"
#include "ThreadPool.h"
#include "HttpHandler.h"

class TCPserver
{
public:
	TCPserver(std::shared_ptr<HttpHandler> sp) :m_http_handler(sp) {}
	void init_tcp_server(std::shared_ptr<EventLoop>);
	void start_tcp_server(int);
	//����ͻ��˵������¼�
	static void handle_connect(void*);
	//static void remove_closed_socket(void*);

private:
	//���ܿͻ��˵�����
	int accept_connect_socket(void);

private:
	ThreadPool m_threadPool;
	std::shared_ptr<EventLoop> m_main_event_loop;
	//std::list<std::shared_ptr<TCPconnection>> m_tcp_connect_lst;
	//std::mutex m_mut;
	//std::thread m_detect_socket_thread;          //�ں�̨��Ⲣɾ���ѹر��׽��ֵ��߳�

	int m_listen_fd;        //�����׽���������
	int m_listen_port;		//�����˿�

	std::shared_ptr<HttpHandler> m_http_handler;  //http�ص�
};

#endif