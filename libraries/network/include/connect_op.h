#pragma once

#include "libraries/common.h"

namespace NetZ
{
  template <typename Handler>
  class ConnectOperation : public ReactorOperation
  {
  public:
    ConnectOperation(const sockaddr_in* conn, SocketHandle fd, CompletionHandler<Handler> _handler)
      : ReactorOperation(fd, &ConnectOperation::DoConnect)
      , socketAddress(conn)
      , handler(std::move(_handler))
    {

    }

    static void DoConnect(ReactorOperation* op, std::error_code& ec)
    {
      static_cast<ConnectOperation*>(op)->CompleteOperation(ec);
    }

    void CompleteOperation(std::error_code& ec)
    {
      handler(ec);
    }

  private:
    const sockaddr_in* socketAddress = nullptr;
    CompletionHandler<Handler> handler;
  };
}
