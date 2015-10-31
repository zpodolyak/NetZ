#pragma once

#include "libraries/common.h"

namespace NetZ
{
  template <typename SocketType, typename Handler>
  class AcceptOperation : public ReactorOperation
  {
  public:
    AcceptOperation(SocketType& _peer, ConnectionData* conn, SocketHandle fd, CompletionHandler<Handler> _handler)
      : ReactorOperation(fd, &AcceptOperation::DoAccept)
      , peer(_peer)
      , connData(conn)
      , handler(std::move(_handler))
    {

    }

    static void DoAccept(ReactorOperation* op, std::error_code& ec)
    {
      AcceptOperation* accOp(static_cast<AcceptOperation*>(op));
      if (accOp->descriptor == INVALID_SOCKET)
        return;

#ifdef WIN32
      int addrLen = 0;
#else
      std::size_t addrLen = 0;
#endif
      sockaddr* addr = nullptr;
      if (accOp->connData)
      {
        addrLen = sizeof(accOp->connData->data);
        addr = (sockaddr*)&accOp->connData->data;
      }
      accOp->peer.Assign(ErrorWrapper(::accept(accOp->descriptor, addr, accOp->connData ? &addrLen : 0), ec));
      accOp->CompleteOperation(ec);
    }

    void CompleteOperation(std::error_code& ec)
    {
      handler(ec);
    }

  private:
    SocketType& peer;
    ConnectionData* connData;
    CompletionHandler<Handler> handler;
  };
}

