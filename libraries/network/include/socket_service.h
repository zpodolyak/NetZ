#pragma once

#include "io_service.h"

namespace NetZ
{

  class SocketService : public IoService
  {
  public:
    SocketService();
    virtual void RegisterDescriptorOperation(int type, ReactorOperation* op) override;
    virtual void CancelDescriptor(SocketHandle fd) override;
    virtual Util::TimerID AddTimer(Util::Timer&& timer) override;
    virtual void CancelTimer(Util::TimerID timerID) override;
    virtual void ResetTimer(Util::TimerID timerID) override;
    virtual void Run() override;
    virtual void Stop() override;
    virtual bool IsRunning() const override;
  private:
    Reactor reactor;
    Util::TimerHost timers;
  };
}