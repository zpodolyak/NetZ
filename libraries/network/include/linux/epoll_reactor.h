#pragma once

namespace NetZ
{
  class Reactor
  {
  public:
    Reactor();
    ~Reactor();

    void RegisterDescriptor(SocketHandle fd);
    void RegisterOperation(int type, ReactorOperation* op);
    void CancelDescriptor(SocketHandle fd);
    bool HasRegisteredOperation(int type, ReactorOperation* op);

    void Run(int timeout = 1000 / 60);
    void Stop() { shutdown = true; }
    bool IsRunning() const { return !shutdown; }
  private:
    int epoll_fd;
    std::unordered_map<SocketHandle, std::deque<ReactorOperation*>> taskQueue[REACTOR_QUEUES_SIZE];
    bool shutdown = false;
  };
}

