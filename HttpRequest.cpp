#include "HttpRequest.h"


#define INIT_REQUEST_HEADER_SIZE 128

const char *HTTP10 = "HTTP/1.0";
const char *HTTP11 = "HTTP/1.1";
const char *KEEP_ALIVE = "Keep-Alive";
const char *CLOSE = "close";
 
//清除一个request对象
void http_request_clear(struct http_request *httpRequest) {
	if (httpRequest->request_headers != NULL) {
		for (int i = 0; i < httpRequest->request_headers_number; i++) {
			free(httpRequest->request_headers[i].key);
			free(httpRequest->request_headers[i].value);
		}
		free(httpRequest->request_headers);
	}
	free(httpRequest);
}

//重置一个request对象
void http_request_reset(struct http_request *httpRequest) {
	httpRequest->method = NULL;
	httpRequest->current_state = REQUEST_STATUS;
	httpRequest->version = NULL;
	httpRequest->url = NULL;
	httpRequest->request_headers_number = 0;
}

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
	char *connection = http_request_get_header(httpRequest, "Connection");

	if (connection != NULL && strncmp(connection, CLOSE, strlen(CLOSE)) == 0) {
		return 1;
	}

	if (httpRequest->version != NULL &&
		strncmp(httpRequest->version, HTTP10, strlen(HTTP10)) == 0 &&
		strncmp(connection, KEEP_ALIVE, strlen(KEEP_ALIVE)) == 1) {
		return 1;
	}
	return 0;
}

void HttpRequest::http_request_reset(void)
{

}

