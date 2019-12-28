#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "TCPserver.h"
#include "HttpHandler.h"

class HttpServer
{
public:
	HttpServer(void) :m_http_handler(new HttpHandlerImpl),
					  m_tcp_server(m_http_handler){}

	void init_http_server(void);
	void start_http_server(void);

private:
	TCPserver m_tcp_server;
	std::shared_ptr<HttpHandler> m_http_handler;
};


#endif
