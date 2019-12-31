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
	//处理客户端的连接事件
	static void handle_connect(void*);
	//static void remove_closed_socket(void*);

private:
	//接受客户端的连接
	int accept_connect_socket(void);

private:
	ThreadPool m_threadPool;
	std::shared_ptr<EventLoop> m_main_event_loop;
	//std::list<std::shared_ptr<TCPconnection>> m_tcp_connect_lst;
	//std::mutex m_mut;
	//std::thread m_detect_socket_thread;          //在后台检测并删除已关闭套接字的线程

	int m_listen_fd;        //监听套接字描述符
	int m_listen_port;		//监听端口

	std::shared_ptr<HttpHandler> m_http_handler;  //http回调
};

#endif