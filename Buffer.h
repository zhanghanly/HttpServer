#ifndef BUFFER_H
#define BUFFER_H

constexpr const char* CRLF = "\r\n";

class Buffer
{
public:
	Buffer(void) {}
	//��Buffer��д����
	int buffer_append(const void* data, int size);
	//��Buffer��д�ַ���
	int buffer_append_string(const char* data);
	//��Buffer��дһ���ַ�
	int buffer_append_char(char data);
	//��socket�����ݲ�д��Buffer
	int buffer_socket_read(int fd);
	//��Buffer�ж�ȡһ���ַ�
	char buffer_read_char(void);
	//��Buffer��Ѱ��CRLF
	char* buffer_find_CRLF(void);
	//Buffer�ɶ��Ĵ�С
	int buffer_readable_size(void);
	//ʹBuffer������size���ַ�
	void make_room(int size);
	//Buffer��д�Ĵ�С
	int buffer_writeable_size(void);
	//Bufferǰ��Ŀ��пռ�
	int buffer_front_spare_size(void);

	char* m_data;       //ʵ�ʵ�����
	int m_totalSize;    //�ܵĴ�С
	int m_writeIndex;   //д���λ��
	int m_readIndex;    //��ȡ��λ��
};


#endif
