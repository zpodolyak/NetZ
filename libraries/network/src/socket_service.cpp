#include "libraries/common.h"
#include "network.h"
#include "socket_service.h"

namespace NetZ
{
  SocketService::SocketService()
    : reactor()
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

  void SocketService::AddTimer(const Util::Timer& timer)
  {

  }

  void SocketService::Run()
  {

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