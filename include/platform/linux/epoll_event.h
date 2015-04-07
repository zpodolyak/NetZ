#ifndef EPOLL_EVENT_H
#define EPOLL_EVENT_H 

class Event
{
	friend class Reactor;
public:
	Event(int fd) : mFd(fd) 
	{
		mEvent.data.fd = mFd;
		mEvent.events = EPOLLIN | EPOLLET; 
	}

private:
	int mFd;
	struct epoll_event mEvent;
};

#endif
