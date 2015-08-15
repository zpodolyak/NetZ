#pragma once

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
    fd_set fds[ReactorOps::max_ops];
    std::unordered_map<SocketHandle, std::deque<ReactorOperation>> taskQueue[REACTOR_QUEUES_SIZE];
  };
}