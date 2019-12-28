#include <iostream>
#include <cstring>
#include "TCPconnectionHandlerImpl.h"

int TCPconnectionHandlerImpl::connection_completed_callback(TCPconnection* tcp)
{
	std::cout << "connection completed " << std::endl;
}

int TCPconnectionHandlerImpl::message_callback(TCPconnection* tcp)
{
	if (m_httpHandler->parse_http_request(tcp->m_input_buffer, m_httpRequest) == 0) {
		const char* error_response = "HTTP/1.1 400 Bad Request\r\n\r\n";
		tcp->tcp_connection_send_data(error_response, strlen(error_response));
		tcp->tcp_connection_shutdown();
	}

	//处理完了所有的request数据，接下来进行编码和发送
	if (m_httpRequest->current_state == http_request_state::REQUEST_DONE) {
		std::shared_ptr<HttpResponse> httpResponse(new HttpResponse);

		//httpServer暴露的requestCallback回调
		if (m_httpHandler)
			m_httpHandler->response_to_request(m_httpRequest, m_httpResponse);
		
		std::shared_ptr<Buffer> buffer(new Buffer);
		m_httpResponse->http_response_encode_buffer(buffer);
		tcp->tcp_connection_send_buffer(buffer);

		if (m_httpRequest->http_request_close_connection()) {
			tcp->tcp_connection_shutdown();
			m_httpRequest->http_request_reset();
		}
	}
}

int TCPconnectionHandlerImpl::write_completed_call_back(TCPconnection* tcp)
{
	std::cout << "write completed" << std::endl;
}

int TCPconnectionHandlerImpl::connection_closed_call_back(TCPconnection* tcp)
{
	std::cout << "connection closed" << std::endl;
}

