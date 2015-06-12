#ifndef SOCKET_H
#define SOCKET_H

namespace Netz
{
  class SocketOption;
   
  class SocketBase
  {
  public:
    SocketBase(const SocketBase&) = delete;
    SocketBase& operator=(const SocketBase&) = delete;

    SocketBase();

    template <typename ProtocolType>
    SocketBase(const ProtocolType& prot)
    {
      Open(prot);
    }

    template <typename ProtocolType>
    void Open(const ProtocolType& prot)
    {
      socket = ::socket(prot.family, prot.type, prot.protocol);
      if(socket == INVALID_SOCKET)
        PrintError("SocketBase::Open");
    }

    void Bind(const ConnectionData& conn);
    int  Connect(const ConnectionData& conn);
    void Close();

    void GetSocketOption(SocketOption& opt) const;
    void SetSocketOption(const SocketOption& opt);

    ConnectionData LocalConnection() const;
    ConnectionData RemoteConnection() const;

    bool IsNonBlocking() const;
    void SetNonBlocking(bool mode);
    
    template <typename ProtocolType>
    static SocketHandle CreateServerSocket(unsigned short port);

    template <typename ProtocolType>
    static SocketHandle CreateClientSocket(unsigned short port, const char* host);
  protected:
    ~SocketBase() {}

    SocketHandle socket;
  };

  template <typename ProtocolType>
  SocketHandle SocketBase::CreateServerSocket(unsigned short port)
  {
    SocketHandle sockfd;
    addrinfo *res = nullptr, *p = nullptr;
    ProtocolType prot;
    
    if (Address::ResolveFromHostname(nullptr, prot.type, std::to_string(port).c_str(), &res))
    {
      for (p=res; p; p=p->ai_next)
      {
        sockfd = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd < 0)  
            continue;
        
        int yes=1;
        ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
      
        if (::bind(sockfd, p->ai_addr, p->ai_addrlen) < 0) 
        {
          ::close(sockfd);
          continue;
        }
        break;
      }
      ::freeaddrinfo(res);
      if(!p)
      {
        PrintError("failed to bind socket!");
        return INVALID_SOCKET;
      }
    }
    else
      return INVALID_SOCKET;
    
    ::listen(sockfd, 10);
    return sockfd;
  }

  template <typename ProtocolType>
  SocketHandle SocketBase::CreateClientSocket(unsigned short port, const char* host)
  {
    addrinfo *res = nullptr, *p = nullptr;
    SocketHandle sockfd;
    ProtocolType prot;
    
    if (Address::ResolveFromHostname(host, prot.type, std::to_string(port).c_str(), &res))
    {
      for (p=res; p; p=p->ai_next)
      {
        if ((sockfd = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
          continue;
        
        if((::connect(sockfd, p->ai_addr, p->ai_addrlen)) < 0)
        {
          PrintError("connect() error");
          ::close(sockfd);
          continue;
        }
        break;
      }
      ::freeaddrinfo(res);
      if(!p)
      {
        PrintError("failed to connect to server");
        return INVALID_SOCKET;
      }
    }
    else
      return INVALID_SOCKET;
    return sockfd;
  }
}

#endif
