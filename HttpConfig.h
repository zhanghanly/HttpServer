#ifndef HTTP_CONFIG_H
#define HTTP_CONFIG_H

#include <string>
#include <mutex>

class HttpConfig
{
public:
	static HttpConfig* get_instance(void);
	static void delete_instance(void);
	bool init(const std::string&);
	std::string get_http_ip(void) { return m_http_ip; }
	int get_http_port(void) { return m_http_port; }
	int get_thread_num(void) { return m_thread_num; }

private:
	HttpConfig(void) = default;

	static HttpConfig* s_config_handler;
	static std::mutex s_mut;
	std::string m_http_ip;
	int m_http_port;
	int m_thread_num;
};

#endif
