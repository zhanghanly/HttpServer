#include "HttpServer.h"
#include "HttpConfig.h"


void HttpServer::init_http_server(void)
{
	m_tcp_server.init_tcp_server(m_event_loop);
}

void HttpServer::start_http_server(void)
{
	//开启的线程数量
	int thread_num = HttpConfig::get_instance()->get_thread_num();
	m_tcp_server.start_tcp_server(thread_num);
}

