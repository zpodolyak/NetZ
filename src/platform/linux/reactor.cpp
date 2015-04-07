#include "common.h"

epoll_event events[MAX_EVENTS];

Reactor::Reactor()
: mOwnerEvent(epoll_create1(0))
, mTotalEvents(0)
{
	if(mOwnerEvent.mFd==-1)
	{
		perror("epoll_create");
	}
}

Reactor::~Reactor()
{
	std::vector<EventHandler>::iterator it=mEventList.begin(), itEnd=mEventList.end();
	for(; it!=itEnd;)
	{
		delete it->first;
	}
}

void Reactor::AddEventHandler(Event *event, const Listener &listener)
{
	if(epoll_ctl(mOwnerEvent.mFd, EPOLL_CTL_ADD, event->mFd, &event->mEvent)!=0)
	{
		perror("epoll_ctl");
		return;
	}	
	mEventList.push_back(std::make_pair(event,listener));
	++mTotalEvents;
}

void Reactor::RemoveEventHandler(Event &event)
{
	if(epoll_ctl(mOwnerEvent.mFd, EPOLL_CTL_DEL, event.mFd, &event.mEvent)!=0)
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
	int n = epoll_wait(mOwnerEvent.mFd, events, MAX_EVENTS, timeout);

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
