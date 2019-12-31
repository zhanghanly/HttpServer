#ifndef TCP_CONNECTION_HANDLER_IMPL_H
#define TCP_CONNECTION_HANDLER_IMPL_H

#include "TCPconnectionHandler.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpHandler.h"

class TCPconnectionHandlerImpl : public TCPconnectionHandler
{
public:
	TCPconnectionHandlerImpl(std::shared_ptr<HttpHandler> sp) : 
		m_httpHandler(sp),
		m_httpRequest(new HttpRequest),
		m_httpResponse(new HttpResponse){
		if (!m_httpHandler)
			std::cout << "*m_httpHandler == nullptr in TCPconnectionHandlerImpl" << std::endl;
	}

	virtual int connection_completed_callback(TCPconnection*) override;
	virtual int message_callback(TCPconnection*) override;
	virtual int write_completed_callback(TCPconnection*) override;
	virtual int connection_closed_callback(TCPconnection*) override;

private:
	std::shared_ptr<HttpRequest> m_httpRequest;
	std::shared_ptr<HttpResponse> m_httpResponse;
	std::shared_ptr<HttpHandler> m_httpHandler;
};

#endif