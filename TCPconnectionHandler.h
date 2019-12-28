#ifndef TCP_CONNECTION_HANDLER_H
#define TCP_CONNECTION_HANDLER_H

#include "TCPconnection.h"

class TCPconnectionHandler
{
public:
	//������ɵĻص�
	virtual int connection_completed_callback(TCPconnection*) = 0;
	//
	virtual int message_callback(TCPconnection*) = 0;
	//���ͻ���д������ɵĻص�
	virtual int write_completed_call_back(TCPconnection*) = 0;
	//�׽��ֹرյĻص�
	virtual int connection_closed_call_back(TCPconnection*) = 0;
};

#endif
