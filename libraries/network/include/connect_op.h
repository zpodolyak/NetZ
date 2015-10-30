#pragma once

#include "libraries/common.h"

namespace NetZ
{
  template <typename Handler>
  class ConnectOperation : public ReactorOperation
  {
  public:
    ConnectOperation(const sockaddr_in* conn, SocketHandle fd, CompletionHandler<Handler> _handler)
      : ReactorOperation(fd, [this](std::error_code& _ec)
    {
      DoConnect(_ec);
    })
      , socketAddress(conn)
      , handler(std::move(_handler))
    {

    }

    void DoConnect(std::error_code& _ec)
    {
      if (descriptor == INVALID_SOCKET || !socketAddress)
        return;
      // do nothing for now; may be worth checking if connection really succeeded
    }

    virtual void CompleteOperation() override
    {
      handler(ec);
    }

  private:
    const sockaddr_in* socketAddress = nullptr;
    CompletionHandler<Handler> handler;
  };
}
