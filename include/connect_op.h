#pragma once

#include "common.h"

namespace Netz
{
  class ConnectOperation : public ReactorOperation
  {
  public:
    ConnectOperation(const sockaddr_in* conn, SocketHandle fd, CompletionHandler _handler)
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
      if (handler)
        handler(ec);
    }

  private:
    const sockaddr_in* socketAddress = nullptr;
    CompletionHandler handler;
  };
}
