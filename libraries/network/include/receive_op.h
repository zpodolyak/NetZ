#pragma once

#include "common.h"

namespace NetZ
{
  class ReceiveOperation : public ReactorOperation
  {
  public:
    ReceiveOperation(char* buff, int len, int msg_flags, SocketHandle fd, DataCompletionHandler _handler)
      : ReactorOperation(fd, [this](std::error_code& _ec)
    {
      DoReceive(_ec);
    })
      , buffer(buff)
      , length(len)
      , flags(msg_flags)
      , desc(fd)
      , handler(std::move(_handler))
    {

    }

    void DoReceive(std::error_code& _ec)
    {
      if (desc == INVALID_SOCKET)
        return;
      bytes_transferred = ErrorWrapper(::recv(desc, buffer, length, flags), _ec);
    }

    virtual void CompleteOperation() override
    {
      if (handler)
        handler(bytes_transferred, ec);
    }

  private:
    char* buffer = nullptr;
    int length, flags, bytes_transferred;
    SocketHandle desc;
    DataCompletionHandler handler;
  };
}

