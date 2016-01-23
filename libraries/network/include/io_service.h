#pragma once

#include "reactor.h"
#include "timer.h"

namespace NetZ
{
  class IoService
  {
  public:
    virtual ~IoService() {}
    virtual void RegisterDescriptor(SocketHandle) = 0;
    virtual void RegisterOperation(int, ReactorOperation*) = 0;
    virtual void CancelDescriptor(SocketHandle) = 0;
    virtual Util::TimerID AddTimer(Util::Timer&& timer) = 0;
    virtual void CancelTimer(Util::TimerID timerID) = 0;
    virtual void ResetTimer(Util::TimerID timerID) = 0;
    virtual void Run() = 0;
    virtual void Stop() = 0;
    virtual bool IsRunning() const = 0;
  };
}
