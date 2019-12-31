#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "TCPserver.h"
#include "HttpHandler.h"

class HttpServer
{
public:
	HttpServer(void) : m_http_handler(new HttpHandlerImpl),
					   m_event_loop(new EventLoop),
		               m_tcp_server(m_http_handler){}

	void init_http_server(void);
	void start_http_server(void);

private:
	std::shared_ptr<EventLoop> m_event_loop;  //处理连接套接字的线程
	std::shared_ptr<HttpHandler> m_http_handler;
	TCPserver m_tcp_server;
};


#endif
