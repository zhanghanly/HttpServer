#include <cstring>
#include <iostream>
#include <string>
#include <cassert>
#include "HttpHandler.h"

void HttpHandlerImpl::response_to_request(std::shared_ptr<HttpRequest>& request, 
										  std::shared_ptr<HttpResponse>& response)
{
	const char* url = request->url.c_str();
	//debug
	//std::cout << "url=" << url << std::endl;
	char* question = (char*)memmem(url, strlen(url), "?", 1);
	//char* path = nullptr;
	std::string path;
	if (question != nullptr) {
		//path = malloc(question - url);
		//strncpy(path, url, question - url);
		path = std::string(url, question - url);
	}
	else {
		//path = malloc(strlen(url));
		//strncpy(path, url, strlen(url));
		path = std::string(url, strlen(url));
	}
	//debug
	//std::cout << "path=" << path << std::endl;
	if (strcmp(path.c_str(), "/") == 0) {
		response->statusCode = HttpStatusCode::OK;
		response->statusMessage = "OK";
		response->contentType = "text/html";
		response->body = "<html><head><title>This is network programming</title></head><body><h1>Hello, network programming</h1></body></html>";
	}
	else if (strcmp(path.c_str(), "/network") == 0) {
		response->statusCode = HttpStatusCode::OK;
		response->statusMessage = "OK";
		response->contentType = "text/plain";
		response->body = "hello, network programming";
	}
	else {
		response->statusCode = HttpStatusCode::NotFound;
		response->statusMessage = "Not Found";
		response->keep_connected = 1;
	}

}


int HttpHandlerImpl::parse_http_request(std::shared_ptr<Buffer>& buffer, 
										std::shared_ptr<HttpRequest>& request)
{
	int ok = 1;
	while (request->current_state != http_request_state::REQUEST_DONE) {
		if (request->current_state == http_request_state::REQUEST_STATUS) {
			char *crlf = buffer->buffer_find_CRLF();
			if (crlf) {
				int request_line_size = process_status_line(buffer->m_data + buffer->m_readIndex, crlf, request);
				if (request_line_size) {
					buffer->m_readIndex += request_line_size;  // request line size
					buffer->m_readIndex += 2;  //CRLF size
					request->current_state = http_request_state::REQUEST_HEADERS;
				}
			}
		}
		else if (request->current_state == http_request_state::REQUEST_HEADERS) {
			char *crlf = buffer->buffer_find_CRLF();
			if (crlf) {
				char* start = buffer->m_data + buffer->m_readIndex;
				int request_line_size = crlf - start;
				char* colon = (char*)memmem(start, request_line_size, ": ", 2);
				if (colon != nullptr) {
					/*char *key = malloc(colon - start + 1);
					strncpy(key, start, colon - start);
					key[colon - start] = '\0';*/
					std::string key(start, colon - start);
					//debug
					//std::cout << "key=" << key << std::endl;
					/*char *value = malloc(crlf - colon - 2 + 1);
					strncpy(value, colon + 1, crlf - colon - 2);
					value[crlf - colon - 2] = '\0';*/
					std::string value(colon + 2, crlf - colon - 1);
					//debug
					//std::cout << "value=" << value << std::endl;
					//把(key, value)添加到请求头
					request->http_request_add_header(key, value);

					buffer->m_readIndex += request_line_size;  //request line size
					buffer->m_readIndex += 2;  //CRLF size
				}
				else {
					//读到这里说明:没找到，就说明这个是最后一行
					buffer->m_readIndex += 2;  //CRLF size
					request->current_state = http_request_state::REQUEST_DONE;
				}
			}
		}
	}
	return ok;
}

int HttpHandlerImpl::process_status_line(char* start, char* end,
										 std::shared_ptr<HttpRequest>& request)
{
	int size = end - start;
	//method
	char* space = (char*)memmem(start, end - start, " ", 1);
	assert(space != nullptr);
	/*int method_size = space - start;
	request->method = malloc(method_size + 1);
	strncpy(httpRequest->method, start, space - start);
	httpRequest->method[method_size + 1] = '\0';*/
	request->method = std::string(start, space - start);

	//url
	start = space + 1;
	space = (char*)memmem(start, end - start, " ", 1);
	assert(space != NULL);
	/*int url_size = space - start;
	httpRequest->url = malloc(url_size + 1);
	strncpy(httpRequest->url, start, space - start);
	httpRequest->url[url_size + 1] = '\0';*/
	request->url = std::string(start, space - start);

	//version
	start = space + 1;
	/*httpRequest->version = malloc(end - start + 1);
	strncpy(httpRequest->version, start, end - start);
	httpRequest->version[end - start + 1] = '\0';
	assert(space != NULL);*/
	request->version = std::string(start, end - start);

	return size;

}
