#ifndef CONCRETE_BUFFER_H
#define CONCRETE_BUFFER_H

#include "Buffer.h"

class ConcreteBuffer : public Buffer
{
public:
	ConcreteBuffer(void);
	~ConcreteBuffer(void) { delete m_data; }
	virtual int buffer_append(void* data, int size) override;
	virtual int buffer_append_string(char* data) override;
	virtual int buffer_append_char(char data) override;
	virtual int buffer_socket_read(int fd) override;
	virtual char buffer_read_char(void) override;
	virtual char* buffer_find_CRLF(void) override;

private:
	//使Buffer能容纳size个字符
	void make_room(int size);
	//Buffer可写的大小
	int buffer_writeable_size(void);
	//Buffer可读的大小
	int buffer_readable_size(void);
	//Buffer前面的空闲空间
	int buffer_front_spare_size(void);

private:
	char* m_data;       //实际的数据
	int m_totalSize;    //总的大小
	int m_writeIndex;   //写入的位置
	int m_readIndex;    //读取的位置
};

#endif
