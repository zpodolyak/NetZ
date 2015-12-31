#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

namespace NetZ
{
  template <typename ProtocolType>
  class ServerSocket : public SocketBase
  {
    typedef ProtocolType protocol_type;

  public:
    ServerSocket()
      : SocketBase()
    {
    }

    ServerSocket(const protocol_type& prot)
     : SocketBase(prot)
    {
    }

    ServerSocket(const protocol_type& prot, const ConnectionData& conn)
      : SocketBase(prot)
    {
      Bind(conn);
      Listen();
    }

    ServerSocket(SocketService* _service, const protocol_type& prot, const ConnectionData& conn)
      : SocketBase(_service, prot)
    {
      Bind(conn);
      Listen();
    }

    void Listen()
    {
      if (socket == INVALID_SOCKET)
        return;

      std::error_code ec;
      if (ErrorWrapper(::listen(socket, SOMAXCONN), ec) < 0)
        PrintError(ec);
    }

    template <typename SocketType>
    void Accept(SocketType& peer, ConnectionData* conn = nullptr)
    {
      if (socket == INVALID_SOCKET)
        return;
      std::error_code ec;
#ifdef WIN32
      int addrLen = 0;
#else
      std::size_t addrLen = 0;
#endif
      sockaddr* addr = nullptr;
      if (conn)
      {
        addrLen = sizeof(conn->data);
        addr = (sockaddr*)&conn->data;
      }
      peer.Assign(ErrorWrapper(::accept(socket, addr, conn ? &addrLen : 0), ec));
      if(ec)
        PrintError(ec);
    }

    template <typename SocketType, typename Handler>
    void Accept(SocketType& peer, ConnectionData conn, Handler&& handler)
    {
      if (socket == INVALID_SOCKET)
        return;
      auto op = new AcceptOperation<SocketType, Handler>(peer, std::move(conn), socket, std::forward<Handler>(handler));
      if ( !peer.IsOpen() )
        service->RegisterDescriptorOperation( ReactorOps::read, op );
      else
        op->CompleteOperation(std::make_error_code(std::errc::already_connected));
    }
  };

  using TcpServerSocket = ServerSocket<Tcp>;
}

#endif 
