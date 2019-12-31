#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <iostream>
#include <memory>
#include "EventLoop.h"
#include "Buffer.h"
#include "Channel.h"
#include "TCPconnectionHandler.h"
#include "TCPconnectionHandlerImpl.h"

class TCPconnection
{
public:
	TCPconnection(int fd, std::shared_ptr<EventLoop> spe, std::shared_ptr<HttpHandler> sph) 
		: m_connected_channel(new Channel(fd, EventType::read, this, 
			&TCPconnection::read_callback, &TCPconnection::write_callback)),        
		  m_event_loop(spe),
		  m_tcp_connect_handler(new TCPconnectionHandlerImpl(sph)),
		  m_input_buffer(new Buffer),
		  m_output_buffer(new Buffer) 
	{
		std::cout << "Add connected channel successfully fd=" << fd << std::endl;
		//�������ӵ�channel�����¼�ѭ��
		m_event_loop->add_channel_event(m_connected_channel);
		
	}
	
	//���ص�
	static void read_callback(void*);
	//д�ص�
	static void write_callback(void*);
	//Ӧ�ò�������
	int tcp_connection_send_data(const void* data, int size);
	int tcp_connection_send_buffer(std::shared_ptr<Buffer>&);
	void tcp_connection_shutdown(void);

	std::shared_ptr<Buffer> m_input_buffer;           //���ͻ�����
	std::shared_ptr<Buffer> m_output_buffer;	      //���ͻ�����
	std::shared_ptr<EventLoop> m_event_loop;          //��������socket��������Ǹ��߳�
	std::shared_ptr<Channel> m_connected_channel;     //�������׽���

	std::shared_ptr<TCPconnectionHandler> m_tcp_connect_handler;    //TCPconnection�ص�
};

#endif
