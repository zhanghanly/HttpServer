#include <iostream>
#include "Buffer.h"
#include "HttpConfig.h"
#include "HttpServer.h"

int main()
{
	//init xml
	if (HttpConfig::get_instance()->init("./HttpServer.xml") == false)
		std::cerr << "init xml error" << std::endl;

	HttpServer httpServer;
	httpServer.init_http_server();
	httpServer.start_http_server();


	 
	return 0;
}