#ifndef TCP_CONNECTION_HANDLER_H
#define TCP_CONNECTION_HANDLER_H


//前置声明, 防止互相包含
class TCPconnection;

class TCPconnectionHandler
{
public:
	//连接完成的回调
	virtual int connection_completed_callback(TCPconnection*) = 0;
	//
	virtual int message_callback(TCPconnection*) = 0;
	//给客户端写数据完成的回调
	virtual int write_completed_callback(TCPconnection*) = 0;
	//套接字关闭的回调
	virtual int connection_closed_callback(TCPconnection*) = 0;
};

#endif
