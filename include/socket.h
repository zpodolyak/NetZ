#ifndef SOCKET_H
#define SOCKET_H

namespace Netz
{
  struct ProtocolData;
  class SocketOption;
   
  class SocketBase
  {
  public:
    SocketBase(const SocketBase&) = delete;
    SocketBase& operator=(const SocketBase&) = delete;

    SocketBase();
    SocketBase(const ProtocolData& prot);

    void Open(const ProtocolData& prot);
    void Bind(const ConnectionData& conn);
    void Connect(const ConnectionData& conn);
    void Close();

    void GetSocketOption(SocketOption& opt) const;
    void SetSocketOption(const SocketOption& opt);

    ConnectionData LocalConnection() const;
    ConnectionData RemoteConnection() const;

    bool IsNonBlocking() const;
    void SetNonBlocking(bool mode);
  protected:
    ~SocketBase() {}

    SocketHandle socket;
  };
}

#endif
