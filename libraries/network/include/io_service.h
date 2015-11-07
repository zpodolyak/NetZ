#pragma once

namespace NetZ
{
  class ReactorOperation;
  class Util::Timer;

  class IoService
  {
  public:
    virtual ~IoService() {}
    virtual void RegisterDescriptorOperation(int, ReactorOperation*) = 0;
    virtual void CancelDescriptor(SocketHandle) = 0;
    virtual void AddTimer(const Util::Timer&) = 0;
    virtual void Run() = 0;
    virtual void Stop() = 0;
    virtual bool IsRunning() const = 0;
  };
}