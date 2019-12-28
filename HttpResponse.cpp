#include "HttpResponse.h"


void HttpResponse::http_response_encode_buffer(std::shared_ptr<Buffer>& output) 
{
	char buf[32];
	snprintf(buf, sizeof(buf), "HTTP/1.1 %d ", statusCode);
	output->buffer_append_string(buf);
	output->buffer_append_string(statusMessage.c_str());
	output->buffer_append_string("\r\n");

	if (keep_connected) {
		output->buffer_append_string("Connection: close\r\n");
	}
	else {
		snprintf(buf, sizeof(buf), "Content-Length: %zd\r\n", body.size());
		output->buffer_append_string(buf);
		output->buffer_append_string("Connection: Keep-Alive\r\n");
	}
	for (auto iter = response_headers.begin(); iter != response_headers.end(); iter++) {
		output->buffer_append_string(iter->first.c_str());
		output->buffer_append_string(": ");
		output->buffer_append_string(iter->second.c_str());
		output->buffer_append_string("\r\n");
	}
	output->buffer_append_string("\r\n");
	output->buffer_append_string(body.c_str());
}


