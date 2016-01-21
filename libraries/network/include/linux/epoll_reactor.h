#pragma once

namespace NetZ
{
  class Reactor
  {
  public:
    Reactor();
    ~Reactor();

    void RegisterDescriptorOperation(int type, ReactorOperation* op);
    void CancelDescriptor(SocketHandle fd);
    bool HasRegisteredDescriptor(int type, ReactorOperation* op);

    void Run(int timeout = 200);
    void Stop() { shutdown = true; }
    bool IsRunning() const { return !shutdown; }
  private:
    int epoll_fd;
    std::unordered_map<SocketHandle, std::deque<ReactorOperation*>> taskQueue[REACTOR_QUEUES_SIZE];
    bool shutdown = false;
  };
}

