#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>

enum class EventType
{
	read,
	write
};

enum class ChannelType
{
	add,
	remove,
	update
};

class Channel
{
public:
	Channel(int fd, EventType events, void* data, 
		std::function<void(void*)> read_callback, std::function<void(void*)> write_callback):
	m_fd(fd),
	m_data(data),
	m_events(events),
	m_read_callback(read_callback),
	m_write_callback(write_callback),
	m_write_able(false){}

	bool write_event_is_enabled(void) { return m_write_able; }
	void write_event_enable(void) { m_write_able = true; }
	void write_event_disable(void) { m_write_able = false; }
	 
	int m_fd;
	EventType m_events;
	void* m_data;
	std::function<void(void*)> m_read_callback;
	std::function<void(void*)> m_write_callback;
	ChannelType m_channel_type;
	bool m_write_able;     //ÊÇ·ñ¿ÉÐ´
};

#endif
