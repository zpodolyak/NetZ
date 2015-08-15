#ifndef EPOLL_REACTOR_H
#define EPOLL_REACTOR_H

namespace Netz
{
  class Reactor
  {
  public:
    Reactor();
    ~Reactor();

    void RegisterDescriptor(int type, SocketHandle fd, ReactorOperation&& op);
    void CancelDescriptor(SocketHandle fd);
    void Run(int timeout);
    void Stop();
  private:
    int reactorFD;
    std::unordered_map<SocketHandle, std::deque<ReactorOperation>> taskQueue[REACTOR_QUEUES_SIZE];
  };
}

#endif
