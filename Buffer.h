#ifndef BUFFER_H
#define BUFFER_H

constexpr const char* CRLF = "\r\n";

class Buffer
{
public:
	~Buffer(void) {}
	//��Buffer��д����
	virtual int buffer_append(void* data, int size) = 0;
	//��Buffer��д�ַ���
	virtual int buffer_append_string(char* data) = 0;
	//��Buffer��дһ���ַ�
	virtual int buffer_append_char(char data) = 0;
	//��socket�����ݲ�д��Buffer
	virtual int buffer_socket_read(int fd) = 0;
	//��Buffer�ж�ȡһ���ַ�
	virtual char buffer_read_char(void) = 0;
	//��Buffer��Ѱ��CRLF
	virtual char* buffer_find_CRLF(void) = 0;

	//����һ�������Buffer����
	static Buffer* createBuffer(void);
};


#endif
