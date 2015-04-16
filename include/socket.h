#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

namespace Netz
{
  class Socket
  {
  public:
    Socket(Reactor &_reactor, int socket_type, unsigned short port);

    void OnIncomingConnection(int fd);
    void OnSocketEvent(int fd);

    void Listen();
    int  Accept(Address &client);
    int  ReadStream(char *buffer, int buff_length);
    int  WriteStream(const char *data, int length);
    void SetSocketOptions();
  private:
    int CreateServerSocket();
    
    Reactor &reactor;
    int serverType;

    unsigned short serverPort;
    int socket_fd;
  };
}

#endif
