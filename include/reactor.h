#ifndef REACTOR_H
#define REACTOR_H

class Reactor
{
	typedef std::function<void()> Listener;

	struct EventHandlerComp
	{
		EventHandlerComp(const Event &e) : ev(e) {}

		bool operator()(const std::pair<Event*,Listener> &pr)
		{
			return pr.first->mFd==ev.mFd;
		}
		Event ev;
	};
public:
	Reactor();
	~Reactor();

	void AddEventHandler(Event *event, const  Listener &listener);
	void RemoveEventHandler(Event &event);
	bool DispatchEvents(int timeout=-1);
private:
	int epoll_fd;
	typedef std::pair<Event*,Listener> EventHandler;
	std::vector<EventHandler> mEventList;
	size_t mTotalEvents;
};

#endif