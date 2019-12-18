#include "HttpConfig.h"
#include "Markup.h"

HttpConfig* HttpConfig::s_config_handler = nullptr;
std::mutex HttpConfig::s_mut;

bool HttpConfig::init(const std::string& xml_file_path)
{
	CMarkup makeup;
	if (makeup.Load(xml_file_path) == false)
		return false;
	if (makeup.FindElem("http") == false) return false;
	makeup.IntoElem();

	if (makeup.FindElem("ip") == false)return false;
	m_http_ip = makeup.GetData();
	if (makeup.FindElem("port") == false) return false;
	m_http_port = atoi(makeup.GetData().c_str());
	if (makeup.FindElem("threadNum") == false) return false;
	m_thread_num = atoi(makeup.GetData().c_str());

	makeup.OutOfElem();
	return true;
}

HttpConfig* HttpConfig::get_instance(void)
{
	std::lock_guard<std::mutex> lg(s_mut);
	if (s_config_handler == nullptr) 
		s_config_handler = new HttpConfig();

	return s_config_handler;
}

void HttpConfig::delete_instance(void)
{
	std::lock_guard<std::mutex> lg(s_mut);
	if (s_config_handler != nullptr)
		delete s_config_handler;
}
