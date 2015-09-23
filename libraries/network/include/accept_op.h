#pragma once

#include "common.h"

namespace NetZ
{
  template <typename SocketType>
  class AcceptOperation : public ReactorOperation
  {
  public:
    AcceptOperation(SocketType& _peer, ConnectionData* conn, SocketHandle fd, AcceptCompletionHandler<SocketType> _handler)
      : ReactorOperation(fd, [this](std::error_code& _ec)
    {
      DoAccept(_ec);
    })
      , peer(_peer)
      , connData(conn)
      , handler(std::move(_handler))
    {

    }

    void DoAccept(std::error_code& _ec)
    {
      if (descriptor == INVALID_SOCKET)
        return;

#ifdef WIN32
      int addrLen = 0;
#else
      std::size_t addrLen = 0;
#endif
      sockaddr* addr = nullptr;
      if (connData)
      {
        addrLen = sizeof(connData->data);
        addr = (sockaddr*)&connData->data;
      }
      peer.Assign(ErrorWrapper(::accept(descriptor, addr, connData ? &addrLen : 0), ec));
    }

    virtual void CompleteOperation() override
    {
      if (handler)
        handler(peer, ec);
    }

  private:
    SocketType& peer;
    ConnectionData* connData;
    AcceptCompletionHandler<SocketType> handler;
  };
}

