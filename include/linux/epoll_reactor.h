#ifndef EPOLL_REACTOR_H
#define EPOLL_REACTOR_H

namespace Netz
{
  using event_handle = std::uintptr_t;
  using CompletionHandler = std::function<void()>;

  class Reactor
  {
  public:
    Reactor();
    ~Reactor();

    void AddIoHandler(event_handle event, const CompletionHandler &listener);
    void CancelEvent(event_handle event);
    bool Run(int timeout);
    void Stop();
  private:
    int reactorFD;
    std::unordered_map<event_handle, CompletionHandler> eventMap;
    event_handle eventCounter;
  };
}

#endif
