#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include "HttpRequest.h"
#include "HttpResponse.h"

class HttpHandler
{
public:
	//����request
	virtual int parse_http_request(std::shared_ptr<Buffer>&, std::shared_ptr<HttpRequest>&) = 0;
	//��������λ�Ӧ
	virtual void response_to_request(std::shared_ptr<HttpRequest>&, std::shared_ptr<HttpResponse>&) = 0;
};

class HttpHandlerImpl : public HttpHandler
{
public:
	virtual void response_to_request(std::shared_ptr<HttpRequest>&, std::shared_ptr<HttpResponse>&) override;
	virtual int parse_http_request(std::shared_ptr<Buffer>&, std::shared_ptr<HttpRequest>&) override;
private:
	int process_status_line(char*, char*, std::shared_ptr<HttpRequest>&);
};

#endif
