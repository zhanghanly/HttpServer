#include <new>		  //for  std::nothrow
#include <stdexcept>
#include <exception>
#include <sys/uio.h>  //for readv
#include <cstring>    //for memcpy
#include "ConcreteBuffer.h"

constexpr int buffer_init_size = 65536;

Buffer* Buffer::createBuffer(void)
{
	Buffer* ptr = new ConcreteBuffer();
	return ptr;
}

ConcreteBuffer::ConcreteBuffer(void)
{
	m_data = new (std::nothrow) char[buffer_init_size];
	if (m_data == nullptr)
		throw std::bad_alloc();

	m_totalSize = buffer_init_size;
	m_readIndex = 0;
	m_writeIndex = 0;
}

int ConcreteBuffer::buffer_append(void* data, int size)
{
	if (data != nullptr) {
		make_room(size);
		//拷贝数据到可写空间中
		memcpy(m_data + m_writeIndex, data, size);
		m_writeIndex += size;
	}
}

int ConcreteBuffer::buffer_append_string(char* data)
{
	if (data != nullptr) {
		int size = strlen(data);
		buffer_append(data, size);
	}
}

int ConcreteBuffer::buffer_append_char(char data)
{
	make_room(1);
	//拷贝数据到可写空间中
	m_data[m_writeIndex++] = data;
}

int ConcreteBuffer::buffer_socket_read(int fd)
{
	char additional_buffer[buffer_init_size];
	struct iovec vec[2];
	int max_writable = buffer_writeable_size();
	vec[0].iov_base = m_data + m_writeIndex;
	vec[0].iov_len = max_writable;
	vec[1].iov_base = additional_buffer;
	vec[1].iov_len = sizeof(additional_buffer);
	int result = readv(fd, vec, 2);
	if (result < 0) {
		return -1;
	}
	else if (result <= max_writable) {
		m_writeIndex += result;
	}
	else {
		m_writeIndex = m_totalSize;
		buffer_append(additional_buffer, result - max_writable);
	}
	return result;
}

char ConcreteBuffer::buffer_read_char(void)
{
	char c = m_data[m_readIndex];
	m_readIndex++;
	return c;
}

char* ConcreteBuffer::buffer_find_CRLF(void)
{
	char *crlf = (char*)memmem(m_data + m_readIndex, buffer_readable_size(), CRLF, 2);
	return crlf;
}

void ConcreteBuffer::make_room(int size)
{
	if (buffer_writeable_size() >= size) {
		return;
	}
	//如果front_spare和writeable的大小加起来可以容纳数据，则把可读数据往前面拷贝
	if (buffer_front_spare_size() + buffer_writeable_size() >= size) {
		int readable = buffer_readable_size();
		int i;
		for (i = 0; i < readable; i++) {
			memcpy(m_data + i, m_data + m_readIndex + i, 1);
		}
		m_readIndex = 0;
		m_writeIndex = readable;
	}
	else {
		//扩大缓冲区
		void *tmp = realloc(m_data, m_totalSize + size);
		if (tmp == nullptr) {
			return;
		}
		m_data = (char*)tmp;
		m_totalSize += size;
	}
}

int ConcreteBuffer::buffer_writeable_size(void)
{
	return m_totalSize - m_writeIndex;
}

int ConcreteBuffer::buffer_readable_size(void)
{
	return m_writeIndex - m_readIndex;
}

int ConcreteBuffer::buffer_front_spare_size(void)
{
	return m_readIndex;
}
