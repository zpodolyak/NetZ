#ifndef REACTOR_H
#define REACTOR_H

namespace Netz
{
#if defined(HAS_EPOLL)
# include "linux/epoll_reactor.h"
#else
  class Reactor
  {
    typedef std::function<void(int)> Listener;

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

    void AddEventHandler(Event *event, const Listener &listener);
    void RemoveEventHandler(Event &event);
    bool DispatchEvents(int timeout);
  private:
    int reactorFD;
    typedef std::pair<Event*,Listener> EventHandler;
    std::vector<EventHandler> eventList;
    uint32_t eventCount;
  };
#endif
}
#endif
