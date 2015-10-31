#pragma once

namespace NetZ
{
  class Reactor : public SocketService
  {
  public:
    Reactor();
    ~Reactor();

    virtual void RegisterDescriptorOperation(int type, ReactorOperation* op) override;
    virtual void CancelDescriptor(SocketHandle fd) override;

    void Run(int timeout = 200);
    void Stop();
    bool IsRunning() const { return !shutdown; }
  private:
    fd_set fds[ReactorOps::max_ops];
    std::unordered_map<SocketHandle, std::deque<ReactorOperation*>> taskQueue[REACTOR_QUEUES_SIZE];
    bool shutdown = false;
  };
}
