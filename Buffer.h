#ifndef BUFFER_H
#define BUFFER_H

constexpr const char* CRLF = "\r\n";

class Buffer
{
public:
	~Buffer(void) {}
	//往Buffer里写数据
	virtual int buffer_append(void* data, int size) = 0;
	//往Buffer里写字符串
	virtual int buffer_append_string(char* data) = 0;
	//往Buffer里写一个字符
	virtual int buffer_append_char(char data) = 0;
	//读socket的数据并写入Buffer
	virtual int buffer_socket_read(int fd) = 0;
	//从Buffer中读取一个字符
	virtual char buffer_read_char(void) = 0;
	//在Buffer中寻找CRLF
	virtual char* buffer_find_CRLF(void) = 0;

	//创建一个具体的Buffer对象
	static Buffer* createBuffer(void);
};


#endif
