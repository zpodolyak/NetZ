#pragma once

#include "libraries/common.h"

namespace NetZ
{
  template <typename Handler>
  class ReceiveOperation : public ReactorOperation
  {
  public:
    ReceiveOperation(char* buff, int len, int msg_flags, SocketHandle fd, CompletionHandler<Handler> _handler)
      : ReactorOperation(fd, &ReceiveOperation::DoReceive)
      , buffer(buff)
      , length(len)
      , flags(msg_flags)
      , handler(std::move(_handler))
    {

    }

    static void DoReceive(ReactorOperation* op, std::error_code& ec)
    {
      ClearLastError();
      ReceiveOperation* rcvOp(static_cast<ReceiveOperation*>(op));
      rcvOp->bytes_transferred = ErrorWrapper(::recv(rcvOp->descriptor, rcvOp->buffer, rcvOp->length, rcvOp->flags), ec);
      if (rcvOp->bytes_transferred == 0)
        ec = std::make_error_code(std::errc::connection_aborted);
      rcvOp->CompleteOperation(ec);
    }

    void CompleteOperation(std::error_code& ec)
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

