#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

namespace Netz
{
  struct Socket
  {
  public:
    enum Mode
    {
      Server,
      Client
    };

    void SetNonBlocking();
    int Read(uint8_t *buffer, int length);
    int Write(uint8_t *data, int length);

    static std::shared_ptr<Socket> CreateServerSocket(int type, uint16_t port);
    static std::shared_ptr<Socket> CreateClientSocket(int type, uint16_t port, const char* host);

    Mode m;
    int socket;
  private:
    Socket(Mode mode);
  };
}

#endif
