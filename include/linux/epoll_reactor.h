#ifndef EPOLL_REACTOR_H
#define EPOLL_REACTOR_H

namespace Netz
{
  class Reactor
  {
  public:
    Reactor();
    ~Reactor();

    void RegisterDescriptor(int type, SocketHandle fd, ReactorOperation* op);
    void CancelDescriptor(SocketHandle fd);
    bool Run(int timeout);
    void Stop();
  private:
    int reactorFD;
    std::deque<ReactorOperation> taskQueue[REACTOR_QUEUE_SIZE];
  };
}

#endif
