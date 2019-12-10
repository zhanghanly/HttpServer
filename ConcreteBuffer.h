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
	//ʹBuffer������size���ַ�
	void make_room(int size);
	//Buffer��д�Ĵ�С
	int buffer_writeable_size(void);
	//Buffer�ɶ��Ĵ�С
	int buffer_readable_size(void);
	//Bufferǰ��Ŀ��пռ�
	int buffer_front_spare_size(void);

private:
	char* m_data;       //ʵ�ʵ�����
	int m_totalSize;    //�ܵĴ�С
	int m_writeIndex;   //д���λ��
	int m_readIndex;    //��ȡ��λ��
};

#endif
