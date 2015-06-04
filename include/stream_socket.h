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
      return -1;
    }

    int Receive(char* buffer, int length)
    {
      return -1;
    }
  };

  using TcpSocket = StreamSocket<ProtocolData<Protocol::TCP>>;

}

#endif
