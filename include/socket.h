#ifndef SOCKET_H
#define SOCKET_H

namespace Netz
{
  class Socket
  {
  public:
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    void SetNonBlocking();

    static std::shared_ptr<Socket> CreateServerSocket(int type, uint16_t port);
    static std::shared_ptr<Socket> CreateClientSocket(int type, uint16_t port, const char* host);

    int socket;
  private:
    Socket();
  };
}

#endif
