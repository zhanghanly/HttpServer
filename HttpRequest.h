#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <map>

enum class http_request_state 
{
	REQUEST_STATUS,    //等待解析状态行
	REQUEST_HEADERS,   //等待解析headers
	REQUEST_BODY,      //等待解析请求body
	REQUEST_DONE       //解析完成
};

class HttpRequest
{
public:
	HttpRequest(void) : current_state(http_request_state::REQUEST_STATUS),
		request_headers_number(0) {}

	//根据key返回对应的value
	std::string http_request_get_header(const std::string&);
	//给request增加header
	void http_request_add_header(const std::string&, const std::string&);
	//根据request请求判断是否需要关闭服务器-->客户端单向连接
	int http_request_close_connection(void);
	//重置一个request请求
	void http_request_reset(void);

	std::string version;
	std::string method;
	std::string url;
	http_request_state current_state;
	std::map<std::string, std::string> stdrequest_headers;
	int request_headers_number;
};

#endif
