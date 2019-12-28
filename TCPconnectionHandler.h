#ifndef TCP_CONNECTION_HANDLER_H
#define TCP_CONNECTION_HANDLER_H


//ǰ������, ��ֹ�ظ�����
class TCPconnection;

class TCPconnectionHandler
{
public:
	//������ɵĻص�
	virtual int connection_completed_callback(TCPconnection*) = 0;
	//
	virtual int message_callback(TCPconnection*) = 0;
	//���ͻ���д������ɵĻص�
	virtual int write_completed_callback(TCPconnection*) = 0;
	//�׽��ֹرյĻص�
	virtual int connection_closed_callback(TCPconnection*) = 0;
};

#endif