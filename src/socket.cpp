#include "common.h"

namespace Netz
{
  Socket::Socket()
  {
  }

  Socket::Socket(const ProtocolData& prot)
  {
    Open(prot);
  }

  SocketHandle Socket::Open(const ProtocolData& prot)
  {
    SocketHandle s = ::socket(prot.family, prot.type, prot.protocol);
    if(s < 0)
    {
      PrintError("Socket::Open");
      return INVALID_SOCKET;
    }
    return s;
  }

  SocketHandle Socket::CreateServerSocket(int type, uint16_t port)
  {
    SocketHandle sockfd;
    addrinfo *res = nullptr, *p = nullptr;
    
    if (Address::ResolveFromHostname(nullptr, type, std::to_string(port).c_str(), &res))
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

  SocketHandle Socket::CreateClientSocket(int type, uint16_t port, const char* host)
  {
    addrinfo *res = nullptr, *p = nullptr;
    SocketHandle sockfd;
    
    if (Address::ResolveFromHostname(host, type, std::to_string(port).c_str(), &res))
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

  void Socket::SetNonBlocking(SocketHandle socket)
  {
    int flags = ::fcntl(socket, F_GETFL, 0);

    if (flags < 0 || ::fcntl(socket, F_SETFL, flags | O_NONBLOCK) < 0)
      PrintError("fcntl failed");
  }

}
