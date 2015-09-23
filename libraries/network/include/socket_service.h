#pragma once

namespace NetZ
{
  class ReactorOperation;

  class SocketService
  {
  public:
    virtual ~SocketService() {}
    virtual void RegisterDescriptorOperation(int type, ReactorOperation* op) = 0;
    virtual void CancelDescriptor(SocketHandle fd) = 0;
    virtual void CompleteOperation(ReactorOperation* op) = 0;
  };
}