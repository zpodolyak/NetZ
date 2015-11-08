#pragma once

#include "reactor.h"
#include "timer.h"

namespace NetZ
{
  class IoService
  {
  public:
    virtual ~IoService() {}
    virtual void RegisterDescriptorOperation(int, ReactorOperation*) = 0;
    virtual void CancelDescriptor(SocketHandle) = 0;
    virtual Util::Timer* AddTimer(Util::Timer&& timer) = 0;
    virtual void RemoveTimer(Util::Timer* timer) = 0;
    virtual void Run() = 0;
    virtual void Stop() = 0;
    virtual bool IsRunning() const = 0;
  };
}