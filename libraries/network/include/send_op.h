#pragma once

#include "libraries/common.h"

namespace NetZ
{
  class SendOperation : public ReactorOperation
  {
  public:
    SendOperation(const char* buff, int len, int msg_flags, SocketHandle fd, DataCompletionHandler _handler)
      : ReactorOperation(fd, [this](std::error_code& _ec)
    {
      DoSend(_ec);
    })
      , buffer(buff)
      , length(len)
      , flags(msg_flags)
      , handler(std::move(_handler))
    {

    }

    void DoSend(std::error_code& _ec)
    {
      if (descriptor == INVALID_SOCKET)
        return;
      bytes_transferred = ErrorWrapper(::send(descriptor, buffer, length, flags), _ec);
    }

    virtual void CompleteOperation() override
    {
      if (handler)
        handler(bytes_transferred, ec);
    }

  private:
    const char* buffer = nullptr;
    int length, flags, bytes_transferred;
    DataCompletionHandler handler;
  };
}
