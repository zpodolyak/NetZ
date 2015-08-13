#pragma once

namespace Netz
{
  class Reactor
  {
  public:
    Reactor();
    ~Reactor();

    void RegisterDescriptor(int type, SocketHandle fd, ReactorOperation* op);
    void CancelDescriptor(SocketHandle fd);
    void Run(int timeout);
    void Stop();
  private:
    fd_set fds[static_cast<unsigned>(ReactorOps::max_ops)];
    std::deque<ReactorOperation> taskQueue[REACTOR_QUEUE_SIZE];
  };
}