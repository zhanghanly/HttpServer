#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>
#include <map>
#include <memory>
#include "Buffer.h"

enum class HttpStatusCode 
{
	Unknown,
	OK = 200,
	MovedPermanently = 301,
	BadRequest = 400,
	NotFound = 404,
};

class HttpResponse
{
public:
	HttpResponse(void) : statusCode(HttpStatusCode::Unknown),
		keep_connected(0) {}
	void http_response_encode_buffer(std::shared_ptr<Buffer>&);

	HttpStatusCode statusCode;
	std::string statusMessage;
	std::string contentType;
	std::string body;
	int response_headers_number;
	int keep_connected;
	std::map<std::string, std::string> response_headers;
};

#endif
