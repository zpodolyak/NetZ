#ifndef STREAM_SOCKET_H
#define STREAM_SOCKET_H

#include "protocol.h"

namespace NetZ
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

    StreamSocket(SocketService* _service)
      : SocketBase(_service)
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
#ifndef WIN32
      if (ec == std::error_code((int)std::errc::resource_unavailable_try_again, std::system_category())
        || ec == std::error_code((int)std::errc::operation_would_block, std::system_category()))
#else
      if (ec == std::errc::resource_unavailable_try_again
        || ec == std::errc::operation_would_block)
#endif
        return SOCKET_ERROR;
      return bytes;
    }

    template <typename Handler>
    void Send(const char* buffer, int length, int msg_flags, Handler&& handler)
    {
      if (socket == INVALID_SOCKET || !service)
        return;
      service->RegisterDescriptorOperation(ReactorOps::write, new SendOperation<Handler>(buffer, length, msg_flags, socket, std::forward<Handler>(handler)));
    }

    int Receive(char* buffer, int length, int msg_flags = 0)
    {
      std::error_code ec;
      int bytes = ErrorWrapper(::recv(socket, buffer, length, msg_flags), ec);
#ifndef WIN32
      if (ec == std::error_code((int)std::errc::resource_unavailable_try_again, std::system_category())
        || ec == std::error_code((int)std::errc::operation_would_block, std::system_category()))
#else
      if (ec == std::errc::resource_unavailable_try_again
        || ec == std::errc::operation_would_block)
#endif
        return SOCKET_ERROR;
      return bytes;
    }


    template <typename Handler>
    void Receive(char* buffer, int length, int msg_flags, Handler&& handler)
    {
      if (socket == INVALID_SOCKET || !service)
        return;
      service->RegisterDescriptorOperation(ReactorOps::read, new ReceiveOperation<Handler>(buffer, length, msg_flags, socket, std::forward<Handler>(handler)));
    }
  };

  using TcpSocket = StreamSocket<Tcp>;

}

#endif
