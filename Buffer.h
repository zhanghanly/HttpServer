#ifndef BUFFER_H
#define BUFFER_H

constexpr const char* CRLF = "\r\n";

class Buffer
{
public:
	Buffer(void) {}
	//往Buffer里写数据
	int buffer_append(const void* data, int size);
	//往Buffer里写字符串
	int buffer_append_string(const char* data);
	//往Buffer里写一个字符
	int buffer_append_char(char data);
	//读socket的数据并写入Buffer
	int buffer_socket_read(int fd);
	//从Buffer中读取一个字符
	char buffer_read_char(void);
	//在Buffer中寻找CRLF
	char* buffer_find_CRLF(void);
	//Buffer可读的大小
	int buffer_readable_size(void);
	//使Buffer能容纳size个字符
	void make_room(int size);
	//Buffer可写的大小
	int buffer_writeable_size(void);
	//Buffer前面的空闲空间
	int buffer_front_spare_size(void);

	char* m_data;       //实际的数据
	int m_totalSize;    //总的大小
	int m_writeIndex;   //写入的位置
	int m_readIndex;    //读取的位置
};


#endif
