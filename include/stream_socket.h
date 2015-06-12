#ifndef STREAM_SOCKET_H
#define STREAM_SOCKET_H

#include "protocol.h"

namespace Netz
{
  template <typename ProtocolType>
  class StreamSocket : public SocketBase
  {
    typedef ProtocolType procotol_type;

  public:
    int Send(const char* buffer, int length)
    {
      std::error_code ec;
      int bytes = ErrorWrapper(::send(socket, buffer, length), ec);
      if(ec == std::errc::operation_would_block
        || ec == std::errc::resource_unavailable_try_again)
        return SOCKET_ERROR;
      return bytes;
    }

    int Receive(char* buffer, int length)
    {
      std::error_code ec;
      int bytes = ErrorWrapper(::recv(socket, buffer, length), ec);
      if(ec == std::errc::operation_would_block
        || ec == std::errc::resource_unavailable_try_again)
        return SOCKET_ERROR;
      return bytes;
    }
  };

  using TcpSocket = StreamSocket<ProtocolData<Protocol::TCP>>;

}

#endif
