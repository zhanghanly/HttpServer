#include <iostream>
#include "TCPconnection.h"
#include "Common.h"

void TCPconnection::read_callback(void* data)
{
	TCPconnection* ptr = (TCPconnection*)data;
	if (ptr->m_input_buffer->buffer_socket_read(ptr->m_connected_channel->m_fd) > 0) {
		ptr->m_tcp_connect_handler->message_callback(ptr);
	}
	//读socket发生错误
	else {
		close(ptr->m_connected_channel->m_fd);
		ptr->m_event_loop->remove_channel_event(ptr->m_connected_channel);
		delete ptr;
	}
}

void TCPconnection::write_callback(void* data)
{
	TCPconnection* ptr = (TCPconnection*)data;

	std::shared_ptr<Buffer> output_buffer = ptr->m_output_buffer;
	std::shared_ptr<Channel> channel = ptr->m_connected_channel;

	ssize_t nwrited = write(channel->m_fd, output_buffer->m_data + output_buffer->m_readIndex,
		output_buffer->buffer_readable_size());
	if (nwrited > 0) {
		//已读nwrited字节
		output_buffer->m_readIndex += nwrited;
		//如果数据完全发送出去，就不需要继续了
		if (output_buffer->buffer_readable_size() == 0) {
			channel->write_event_disable();
		}
		//回调writeCompletedCallBack
		/*if (ptr->writeCompletedCallBack != NULL) {
			tcpConnection->writeCompletedCallBack(tcpConnection);
		}*/
		ptr->m_tcp_connect_handler->write_completed_callback(ptr);
	}
	else {
		std::cout << "handle_write for tcp connection" << std::endl; //tcpConnection->name);
	}


}

//应用层调用入口
int TCPconnection::tcp_connection_send_data(const void* data, int size) 
{
	size_t nwrited = 0;
	size_t nleft = size;
	int fault = 0;

	//先往套接字尝试发送数据
	if (!m_connected_channel->write_event_is_enabled() && m_output_buffer->buffer_readable_size() == 0) {
		nwrited = write(m_connected_channel->m_fd, data, size);
		if (nwrited >= 0) {
			nleft = nleft - nwrited;
		}
		else {
			nwrited = 0;
			if (errno != EWOULDBLOCK) {
				if (errno == EPIPE || errno == ECONNRESET) {
					fault = 1;
				}
			}
		}
	}
	if (!fault && nleft > 0) {
		//拷贝到Buffer中，Buffer的数据由框架接管
		m_output_buffer->buffer_append(data + nwrited, nleft);
		if (!m_connected_channel->write_event_is_enabled()) {
			m_connected_channel->write_event_enable();
		}
	}
	return nwrited;
}

int TCPconnection::tcp_connection_send_buffer(std::shared_ptr<Buffer>& buffer) 
{
	int size = buffer->buffer_readable_size();
	int result = tcp_connection_send_data(buffer->m_data + buffer->m_readIndex, size);
	buffer->m_readIndex += size;
	return result;
}


void TCPconnection::tcp_connection_shutdown(void) 
{
	if (shutdown(m_connected_channel->m_fd, SHUT_WR) < 0) {
		std::cerr << "tcp_connection_shutdown failed, socket == " 
		<< m_connected_channel->m_fd << std::endl;
	}
}