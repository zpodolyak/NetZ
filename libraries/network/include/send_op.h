#pragma once

#include "libraries/common.h"

namespace NetZ
{
  template <typename Handler>
  class SendOperation : public ReactorOperation
  {
  public:
    SendOperation(const char* buff, int len, int msg_flags, SocketHandle fd, CompletionHandler<Handler> _handler)
      : ReactorOperation(fd, &SendOperation::DoSend)
      , buffer(buff)
      , length(len)
      , flags(msg_flags)
      , handler(std::move(_handler))
    {

    }

    static void DoSend(ReactorOperation* op, std::error_code& ec)
    {
      SendOperation* sndOp(static_cast<SendOperation*>(op));
      sndOp->bytes_transferred = ErrorWrapper(::send(sndOp->descriptor, sndOp->buffer, sndOp->length, sndOp->flags), ec);
      sndOp->CompleteOperation(ec);
    }

    void CompleteOperation(std::error_code& ec)
    {
      handler(bytes_transferred, ec);
    }

  private:
    const char* buffer = nullptr;
    int length, flags, bytes_transferred;
    CompletionHandler<Handler> handler;
  };
}
