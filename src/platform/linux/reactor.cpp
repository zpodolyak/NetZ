#include "common.h"

Reactor::Reactor()
#if defined(EPOLL_CLOEXEC)
: epoll_fd(epoll_create1(EPOLL_CLOEXEC))
#else
: epoll_fd(-1)
#endif
, mTotalEvents(0)
{
	if(epoll_fd == -1)
	{
    epoll_fd = epoll_create(20000);
    if (epoll_fd != -1)
      ::fcntl(epoll_fd, F_SETFD, FD_CLOEXEC);
    else
		  perror("epoll_create");
	}
}

Reactor::~Reactor()
{
	std::vector<EventHandler>::iterator it=mEventList.begin(), itEnd=mEventList.end();
	for(; it!=itEnd;++it)
	{
		delete it->first;
	}
}

void Reactor::AddEventHandler(Event *event, const Listener &listener)
{
	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event->mFd, &event->mEvent)!=0)
	{
		perror("epoll_ctl");
		return;
	}	
	mEventList.push_back(std::make_pair(event,listener));
	++mTotalEvents;
}

void Reactor::RemoveEventHandler(Event &event)
{
	if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, event.mFd, &event.mEvent)!=0)
	{
		perror("epoll_ctl");
		return;
	}

	for(std::vector<EventHandler>::iterator it=mEventList.begin(), itEnd=mEventList.end();
		it!=itEnd;)
	{
		Event *eptr = it->first; 
		if(eptr->mFd==event.mFd)
		{
			delete it->first;
			it=mEventList.erase(it);
			break;
		}
		else
		{
			++it;
		}	
	}
	--mTotalEvents;	
}


bool Reactor::DispatchEvents(int timeout)
{
  epoll_event events[128];
	int n = epoll_wait(epoll_fd, events, 128, timeout);

	if(!n) 
		return false;

	for(int i=0; i<n; ++i)
	{
		if((events[i].events & EPOLLERR) || 
		   (events[i].events & EPOLLHUP) ||
		   !(events[i].events & EPOLLIN))
		{
			fprintf(stderr, "epoll error\n");
			continue;
		}
		Event ev(events[i].data.fd);
		std::vector<EventHandler>::iterator it=std::find_if(mEventList.begin(), mEventList.end(), EventHandlerComp(ev)); 

		if(it != mEventList.end())
		{
			Listener lst = it->second;
			lst();
		}
	}
	return true;
}
