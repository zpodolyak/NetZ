#include "common.h"
#include "select_reactor.h"

namespace Netz
{
  Reactor::Reactor()
  {

  }

  Reactor::~Reactor()
  {
  }

  void Reactor::RegisterDescriptor(int type, SocketHandle fd, ReactorOperation * op)
  {
  }

  void Reactor::CancelDescriptor(SocketHandle fd)
  {
  }

  void Reactor::Run(int timeout)
  {
  }

  void Reactor::Stop()
  {
  }
}