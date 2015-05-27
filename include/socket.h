#ifndef SOCKET_H
#define SOCKET_H

namespace Netz
{
  struct ProtocolData;
    
  class Socket
  {
  public:
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    void SetNonBlocking(SocketHandle socket);

    static SocketHandle CreateServerSocket(int type, uint16_t port);
    static SocketHandle CreateClientSocket(int type, uint16_t port, const char* host);

    Socket();
    Socket(const ProtocolData& prot);

    SocketHandle Open(const ProtocolData& prot);
  };
}

#endif
