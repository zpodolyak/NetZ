#pragma once

namespace Netz
{
  class Reactor : public SocketService
  {
  public:
    Reactor();
    ~Reactor();

    virtual void RegisterDescriptorOperation(int type, ReactorOperation* op) override;
    virtual void CancelDescriptor(SocketHandle fd) override;
    virtual void CompleteOperation(ReactorOperation* op) override
    {
      if (op)
        op->CompleteOperation();
    }

    void Run(int timeout);
    void Stop() { shutdown = true; }
    bool IsRunning() const { return !shutdown; }
  private:
    int epoll_fd;
    std::unordered_map<SocketHandle, std::deque<ReactorOperation*>> taskQueue[REACTOR_QUEUES_SIZE];
    bool shutdown = false;
  };
}

