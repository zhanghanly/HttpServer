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
	m_write_callback(write_callback){}

	bool write_event_is_enabled(void) { return true; }
	void write_event_enable(void);
	 
	int m_fd;
	EventType m_events;
	void* m_data;
	std::function<void(void*)> m_read_callback;
	std::function<void(void*)> m_write_callback;
	ChannelType m_channel_type;
};

#endif
