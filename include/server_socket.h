#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

namespace Netz
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
      std::size_t addrLen = 0;
      sockaddr* addr = nullptr;
      if (conn)
      {
        addrLen = sizeof(conn->data);
        addr = (sockaddr*)&conn->data;
      }
      peer.socket = ErrorWrapper(::accept(socket, addr, conn ? &addrLen : 0), ec);
      if(ec)
        PrintError(ec);
    }
  };

  using TcpServerSocket = ServerSocket<Tcp>;
}

#endif 
