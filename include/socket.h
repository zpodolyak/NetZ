#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

namespace Netz
{
  typedef struct sockaddr_storage sockaddr_storage;

  class Socket
  {
  public:
    enum Mode
    {
      Server,
      Client
    };

    void SetNonBlocking();

    template<typename Protocol>
    int Read(const Protocol& prot, uint8_t *buffer, int length)
    {
      return prot.Read(buffer, length);
    }

    template<typename Protocol>
    int Write(const Protocol& prot, uint8_t *data, int length)
    {
      return prot.Write(data, length);
    }

    static std::shared_ptr<Socket> CreateServerSocket(int type, uint16_t port);
    static std::shared_ptr<Socket> CreateClientSocket(int type, uint16_t port, const char* host);

    Mode m;
    int socket;
  private:
    Socket(Mode mode);
  };
}

#endif
