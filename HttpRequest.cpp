#include <cstring>
#include "HttpRequest.h"

#define INIT_REQUEST_HEADER_SIZE 128

constexpr const char *HTTP10 = "HTTP/1.0";
constexpr const char *HTTP11 = "HTTP/1.1";
constexpr const char *KEEP_ALIVE = "Keep-Alive";
constexpr const char *CLOSE = "close";
 
//给request增加header
void HttpRequest::http_request_add_header(const std::string& key, const std::string& value)
{
	stdrequest_headers[key] = value;
}

//根据key值获取header熟悉
std::string HttpRequest::http_request_get_header(const std::string& rhs)
{
	return stdrequest_headers[rhs];
}

//根据request请求判断是否需要关闭服务器-->客户端单向连接
int HttpRequest::http_request_close_connection(void) 
{
	std::string value = http_request_get_header("Connection");

	if (value.c_str() != nullptr && strncmp(value.c_str(), CLOSE, strlen(CLOSE)) == 0) {
		return 1;
	}

	if (version.size() != 0 &&
		strncmp(version.c_str(), HTTP10, strlen(HTTP10)) == 0 &&
		strncmp(value.c_str(), KEEP_ALIVE, strlen(KEEP_ALIVE)) == 1) {
		return 1;
	}
	return 0;
}

void HttpRequest::http_request_reset(void)
{
	current_state = http_request_state::REQUEST_STATUS;
	stdrequest_headers.clear();
	request_headers_number = 0;
}

