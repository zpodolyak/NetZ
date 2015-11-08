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

  void SocketService::RegisterDescriptorOperation(int type, ReactorOperation* op)
  {
    reactor.RegisterDescriptorOperation(type, op);
  }

  void SocketService::CancelDescriptor(SocketHandle fd)
  {
    reactor.CancelDescriptor(fd);
  }

  Util::Timer* SocketService::AddTimer(Util::Timer&& timer)
  {
    return timers.Add(std::move(timer));
  }

  void SocketService::RemoveTimer(Util::Timer* timer)
  {
    timers.Remove(timer);
  }

  void SocketService::Run()
  {
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