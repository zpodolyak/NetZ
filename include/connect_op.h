#pragma once

#include "common.h"

namespace Netz
{
  class ConnectOperation : public ReactorOperation
  {
  public:
    ConnectOperation(sockaddr_in* conn, SocketHandle fd, CompletionHandler handler)
      : ReactorOperation(fd, [this] (std::error_code& _ec)
    {
      DoConnect(_ec);
    }
    , handler)
    , socketAddress(conn)
    , desc(fd)
    {

    }

    void DoConnect(std::error_code& _ec)
    {
      if (desc == INVALID_SOCKET || !socketAddress)
        return;
      ErrorWrapper(::connect(desc, (const sockaddr*)socketAddress, sizeof(sockaddr_in)), _ec);
    }

  private:
    sockaddr_in* socketAddress = nullptr;
    SocketHandle desc;
  };
}