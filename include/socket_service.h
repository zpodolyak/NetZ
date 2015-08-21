#pragma once

namespace Netz
{
  class ReactorOperation;

  class SocketService
  {
  public:
    virtual ~SocketService() {}
    virtual void RegisterDescriptor(int type, ReactorOperation&& op) = 0;
    virtual void CancelDescriptor(SocketHandle fd) = 0;
  };
}