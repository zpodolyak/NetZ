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
    SocketBase(SocketService* _service);

    template <typename ProtocolType>
    SocketBase(const ProtocolType& prot)
      : socket(INVALID_SOCKET)
    {
      Open(prot);
    }

    template <typename ProtocolType>
    SocketBase(SocketService* _service, const ProtocolType& prot)
      : socket(INVALID_SOCKET)
      , service(_service)
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
    std::error_code Assign(SocketHandle _socket);
    void Bind(const ConnectionData& conn);

    int Connect(const ConnectionData& conn);

    template <typename Handler>
    void Connect(const ConnectionData& conn, Handler&& handler)
    {
      std::error_code ec;
      const sockaddr_in *socketAddress = &conn.data;
      if (socket == INVALID_SOCKET || !socketAddress || !service)
        return;

      auto op = new ConnectOperation(socketAddress, socket, std::forward<Handler>(handler));
      if (ErrorWrapper(::connect(socket, (const sockaddr*)socketAddress, sizeof(sockaddr_in)), ec) != 0)
      {
        if (ec == std::errc::operation_in_progress
          || ec == std::errc::operation_would_block)
        {
          service->RegisterDescriptorOperation(ReactorOps::connect, op);
        }
      }
      service->CompleteOperation(op);
    }

    void Close();

    std::error_code GetSocketOption(SocketOption& opt) const;
    std::error_code SetSocketOption(const SocketOption& opt);

    ConnectionData LocalConnection() const;
    ConnectionData RemoteConnection() const;

    bool IsNonBlocking() const;
    void SetNonBlocking(bool mode);
 
  protected:
    ~SocketBase() 
    { 
      if (service)
        service->CancelDescriptor(socket);
      Close(); 
    }

    SocketHandle socket;
    SocketService* service = nullptr;
    bool isNonBlocking = false;
  };

  template <typename ProtocolType>
  SocketHandle CreateServerSocket(unsigned short port)
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
          SocketPlatform::Close(sockfd);
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
  SocketHandle CreateClientSocket(unsigned short port, const char* host)
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
          SocketPlatform::Close(sockfd);
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
