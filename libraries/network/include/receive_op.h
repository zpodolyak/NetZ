#pragma once

#include "libraries/common.h"

namespace NetZ
{
  template <typename Handler>
  class ReceiveOperation : public ReactorOperation
  {
  public:
    ReceiveOperation(char* buff, int len, int msg_flags, SocketHandle fd, CompletionHandler<Handler> _handler)
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
      handler(bytes_transferred, ec);
    }

  private:
    char* buffer = nullptr;
    int length, flags, bytes_transferred;
    SocketHandle desc;
    CompletionHandler<Handler> handler;
  };
}

