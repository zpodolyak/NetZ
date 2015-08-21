#ifndef EPOLL_REACTOR_H
#define EPOLL_REACTOR_H

namespace Netz
{
  class Reactor : public SocketService
  {
  public:
    Reactor();
    ~Reactor();

    virtual void RegisterDescriptor(int type, ReactorOperation&& op) override;
    virtual void CancelDescriptor(SocketHandle fd) override;
    void Run(int timeout);
    void Stop();
  private:
    int reactorFD;
    std::unordered_map<SocketHandle, std::deque<ReactorOperation>> taskQueue[REACTOR_QUEUES_SIZE];
  };
}

#endif
