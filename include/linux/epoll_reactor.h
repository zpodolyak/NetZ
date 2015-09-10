#ifndef EPOLL_REACTOR_H
#define EPOLL_REACTOR_H

namespace Netz
{
  class Reactor : public SocketService
  {
  public:
    Reactor();
    ~Reactor();

    virtual void RegisterDescriptor(int type, ReactorOperation* op) override;
    virtual void CancelDescriptor(SocketHandle fd) override;
    void Run(int timeout) { }
    void Stop() { shutdown = true; }
    bool IsRunning() const { return !shutdown; }
  private:
    int epoll_fd;
    std::unordered_map<SocketHandle, std::deque<ReactorOperation*>> taskQueue[REACTOR_QUEUES_SIZE];
    bool shutdown = false;
  };
}

#endif
