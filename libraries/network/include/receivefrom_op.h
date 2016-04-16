#pragma once

#include "libraries/common.h"

namespace NetZ
{
  template <typename Handler, typename ConnData>
  class ReceiveFromOperation : public ReactorOperation
  {
  public:
    ReceiveFromOperation(char* buff, int len, int msg_flags, SocketHandle fd, ConnData& _connData, CompletionHandler<Handler> _handler)
      : ReactorOperation(fd, &ReceiveFromOperation::DoReceiveFrom)
      , buffer(buff)
      , length(len)
      , flags(msg_flags)
      , connData(_connData)
      , handler(std::move(_handler))
    {

    }

    static void DoReceiveFrom(ReactorOperation* op, std::error_code& ec)
    {
      ClearLastError();
      ReceiveFromOperation* rcvOp(static_cast<ReceiveFromOperation*>(op));
      auto connLen = int(rcvOp->connData.Size());
      rcvOp->bytes_transferred = ErrorWrapper(::recvfrom(rcvOp->descriptor, rcvOp->buffer, rcvOp->length, rcvOp->flags,
                                              (sockaddr*)&rcvOp->connData.data, &connLen), ec);
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
    ConnData connData;
    CompletionHandler<Handler> handler;
  };
}
