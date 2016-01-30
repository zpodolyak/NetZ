#include "libraries/common.h"
#include "network.h"
#include "socket_service.h"

namespace NetZ
{
  SocketService::SocketService()
    : reactor()
    , timers()
  {
  }

  void SocketService::RegisterDescriptor(SocketHandle fd)
  {
    reactor.RegisterDescriptor(fd);
  }

  void SocketService::RegisterOperation(int type, ReactorOperation* op)
  {
    if (!reactor.HasRegisteredOperation(type, op))
    {
      reactor.RegisterOperation(type, op);
    }
    else
    {
      waitingOps.push_back(std::make_pair(type, op));
    }
  }

  void SocketService::CancelDescriptor(SocketHandle fd)
  {
    reactor.CancelDescriptor(fd);
  }

  Util::TimerID SocketService::AddTimer(Util::Timer&& timer)
  {
    return timers.Add(std::move(timer));
  }

  void SocketService::CancelTimer(Util::TimerID timerID)
  {
    timers.Cancel(timerID);
  }

  void SocketService::ResetTimer(Util::TimerID timerID)
  {
    timers.Reset(timerID);
  }

  void SocketService::Run()
  {  
    auto it = std::begin(waitingOps);
    while (it != std::end(waitingOps))
    {
      reactor.RegisterOperation(it->first, it->second);
      it = waitingOps.erase(it);
    }
    reactor.Run();
    timers.RunTimers();
  }

  void SocketService::Stop()
  {
    reactor.Stop();
  }

  bool SocketService::IsRunning() const
  {
    return reactor.IsRunning();
  }
}
