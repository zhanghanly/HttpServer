#include <iostream>
#include <errno.h>
#include "TCPserver.h"
#include "HttpConfig.h"
#include "Channel.h"

void TCPserver::init_tcp_server(std::shared_ptr<EventLoop> event_loop)
{
	m_listen_port = HttpConfig::get_instance()->get_http_port();
	m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	//make_nonblocking(acceptor1->listen_fd);
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(m_listen_port);

	int on = 1;
	setsockopt(m_listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	int rt1 = bind(m_listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (rt1 < 0) {
		std::cerr << "bind failed " << strerror(errno) << std::endl;
	}

	int rt2 = listen(m_listen_fd, LISTENQ);
	if (rt2 < 0) {
		std::cerr << "listen failed " << strerror(errno) << std::endl;
	}
	//主线程
	m_main_event_loop = event_loop;
	//初始化主线程
	m_main_event_loop->init_with_name("main");
}

void TCPserver::start_tcp_server(int thread_num)
{
	//创建监听Channel
	std::shared_ptr<Channel> ptr(new Channel(m_listen_fd, EventType::read, this,
		TCPserver::handle_connect, TCPserver::handle_connect));

	//初始化线程池
	m_threadPool.init_thread_pool(m_main_event_loop, thread_num);
	m_threadPool.start_thread_pool();

	//把监听Channel加入主线程
	m_main_event_loop->add_channel_event(ptr);
	m_main_event_loop->start_loop();
	//m_detect_socket_thread = std::thread(&TCPserver::remove_closed_socket, this);
}

void TCPserver::handle_connect(void* data)
{
	TCPserver* ptr = (TCPserver*)data;
	int connected_fd = ptr->accept_connect_socket();
	//从线程池中选出一个线程把已连接套接字加入其中
	std::shared_ptr<EventLoop> sp_loop = (ptr->m_threadPool).select_one_thread_loop();
	//创建已连接socket的对象
	TCPconnection* sp_tcp = new TCPconnection(connected_fd, sp_loop, ptr->m_http_handler);
}

/*void TCPserver::remove_closed_socket(void* data)
{
	TCPserver* TCPptr = (TCPserver*)data;
	
	while (true) {
		std::unique_lock<std::mutex> ul(TCPptr->m_mut);
		auto beg_iter = TCPptr->m_tcp_connect_lst.begin();
		auto end_iter = TCPptr->m_tcp_connect_lst.end();

		while (beg_iter != end_iter) {
			if ((*beg_iter)->is_socket_closed())
				beg_iter = TCPptr->m_tcp_connect_lst.erase(beg_iter);
			++beg_iter;
		}
		ul.unlock();
		//睡眠3分钟
		std::this_thread::sleep_for(std::chrono::minutes(3));
	}
}*/

int TCPserver::accept_connect_socket(void)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int connected_fd = accept(m_listen_fd, (struct sockaddr*)&client_addr, &client_len);

	std::cout << "m_listen_fd=" << m_listen_fd << std::endl;
	std::cout << "connected_fd=" << connected_fd << std::endl;

	//把已连接套接字设置为非阻塞的
	fcntl(connected_fd, F_SETFL, O_NONBLOCK);
	
	return connected_fd;
}






