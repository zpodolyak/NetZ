#ifndef STREAM_SOCKET_H
#define STREAM_SOCKET_H

#include "protocol.h"

namespace Netz
{
  template <typename ProtocolType>
  class StreamSocket : public SocketBase
  {
    typedef ProtocolType protocol_type;

  public:
    StreamSocket()
      : SocketBase()
    {
    }

    StreamSocket(const protocol_type& prot)
      : SocketBase(prot)
    {
    }

    StreamSocket(SocketService* _service, const protocol_type& prot)
      : SocketBase(_service, prot)
    {
    }

    int Send(const char* buffer, int length, int msg_flags)
    {
      std::error_code ec;
      int bytes = ErrorWrapper(::send(socket, buffer, length, msg_flags), ec);
      if (ec == std::errc::operation_would_block
        || ec == std::errc::resource_unavailable_try_again)
        return SOCKET_ERROR;
      return bytes;
    }

    template <typename Handler>
    void Send(const char* buffer, int length, int msg_flags, const Handler& handler)
    {
      if (socket == INVALID_SOCKET || !service)
        return;
      service->RegisterDescriptor(ReactorOps::write, new SendOperation(buffer, length, msg_flags, socket, handler));
    }

    int Receive(char* buffer, int length, int msg_flags = 0)
    {
      std::error_code ec;
      int bytes = ErrorWrapper(::recv(socket, buffer, length, msg_flags), ec);
      if (ec == std::errc::operation_would_block
        || ec == std::errc::resource_unavailable_try_again)
        return SOCKET_ERROR;
      return bytes;
    }


    template <typename Handler>
    void Receive(char* buffer, int length, int msg_flags, const Handler& handler)
    {
      if (socket == INVALID_SOCKET || !service)
        return;
      service->RegisterDescriptor(ReactorOps::read, new ReceiveOperation(buffer, length, msg_flags, socket, handler));
    }
  };

  using TcpSocket = StreamSocket<Tcp>;

}

#endif
