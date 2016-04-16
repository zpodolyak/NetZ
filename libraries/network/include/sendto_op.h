#pragma once

#include "libraries/common.h"

namespace NetZ
{
  template <typename Handler, typename ConnData>
  class SendToOperation : public ReactorOperation
  {
  public:
    SendToOperation(const char* buff, int len, int msg_flags, SocketHandle fd, const ConnData& _connData, CompletionHandler<Handler> _handler)
      : ReactorOperation(fd, &SendToOperation::DoSendTo)
      , buffer(buff)
      , length(len)
      , flags(msg_flags)
      , connData(_connData)
      , handler(std::move(_handler))
    {

    }

    static void DoSendTo(ReactorOperation* op, std::error_code& ec)
    {
      ClearLastError();
      SendToOperation* sndOp(static_cast<SendToOperation*>(op));
      sndOp->bytes_transferred = ErrorWrapper(::sendto(sndOp->descriptor, sndOp->buffer, sndOp->length, sndOp->flags, 
                                              (const sockaddr*)&sndOp->connData.data, sizeof(sockaddr_in)), ec);
      sndOp->CompleteOperation(ec);
    }

    void CompleteOperation(std::error_code& ec)
    {
      handler(bytes_transferred, ec);
    }

  private:
    const char* buffer = nullptr;
    int length, flags, bytes_transferred;
    ConnData connData;
    CompletionHandler<Handler> handler;
  };
}