#ifndef SOCKET_H
#define SOCKET_H

namespace Netz
{
  struct SocketOption
  {
    int level;
    int name;
    int value;

    SocketOption() {}
    SocketOption(int _level, int _name, int _value)
      : level(_level)
      , name(_name)
      , value(_value) {}

    std::size_t Size() const { return sizeof(value); }
    int* ValueData() { return &value; }
    const int* ValueData() const { return &value; }
    bool operator==(const SocketOption& other) const;
  };
   
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
    bool Open(const ProtocolType& prot)
    {
      std::error_code ec;
      socket = ErrorWrapper(::socket(prot.family, prot.type, prot.protocol), ec);
      if(socket == INVALID_SOCKET)
      {
        PrintError(ec);
        return false;
      }
      return true;
    }
    
    bool IsOpen() const;
    void Bind(const ConnectionData& conn);
    int  Connect(const ConnectionData& conn);
    void Close();

    std::error_code GetSocketOption(SocketOption& opt) const;
    std::error_code SetSocketOption(const SocketOption& opt);

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
        ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int));
      
        if (::bind(sockfd, p->ai_addr, p->ai_addrlen) < 0) 
        {
#ifdef WIN32
          ::closesocket(sockfd);
#else
          ::close(sockfd);
#endif
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
#ifdef WIN32
          ::closesocket(sockfd);
#else
          ::close(sockfd);
#endif
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
