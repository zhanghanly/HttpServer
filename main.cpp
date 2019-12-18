#include <iostream>
#include "ConcreteBuffer.h"
#include "HttpConfig.h"

int main()
{
	//init xml
	if (HttpConfig::get_instance()->init("./HttpServer.xml") == false)
		std::cerr << "init xml error" << std::endl;

	std::cout << HttpConfig::get_instance()->get_http_ip() << std::endl;
	std::cout << HttpConfig::get_instance()->get_http_port() << std::endl;
	std::cout << HttpConfig::get_instance()->get_thread_num() << std::endl;

	return 0;
}