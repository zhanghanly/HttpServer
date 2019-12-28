#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <map>

enum class http_request_state 
{
	REQUEST_STATUS,    //�ȴ�����״̬��
	REQUEST_HEADERS,   //�ȴ�����headers
	REQUEST_BODY,      //�ȴ���������body
	REQUEST_DONE       //�������
};

class HttpRequest
{
public:
	HttpRequest(void) : current_state(http_request_state::REQUEST_STATUS),
		request_headers_number(0) {}

	//����key���ض�Ӧ��value
	std::string http_request_get_header(const std::string&);
	//��request����header
	void http_request_add_header(const std::string&, const std::string&);
	//����request�����ж��Ƿ���Ҫ�رշ�����-->�ͻ��˵�������
	int http_request_close_connection(void);
	//����һ��request����
	void http_request_reset(void);

	std::string version;
	std::string method;
	std::string url;
	http_request_state current_state;
	std::map<std::string, std::string> stdrequest_headers;
	int request_headers_number;
};

#endif
