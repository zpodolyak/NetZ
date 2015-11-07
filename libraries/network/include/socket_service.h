#pragma once

#include "io_service.h"
#include "reactor.h"
#include "libraries/timer.h"

namespace NetZ
{

  class SocketService : public IoService
  {
  public:
    SocketService();
    virtual void RegisterDescriptorOperation(int type, ReactorOperation* op) override;
    virtual void CancelDescriptor(SocketHandle fd) override;
    virtual void AddTimer(const Util::Timer& timer) override;
    virtual void Run() override;
    virtual void Stop() override;
    virtual bool IsRunning() const override;
  private:
    Reactor reactor;
    Util::TimerHost timers;
  };
}