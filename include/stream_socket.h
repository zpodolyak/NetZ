#ifndef STREAM_SOCKET_H
#define STREAM_SOCKET_H

#include "protocol.h"

namespace Netz
{
  template <Protocol pr>
  class StreamSocket : public SocketBase
  {
  public:
    static SocketHandle CreateServerSocket(unsigned short port);
    static SocketHandle CreateClientSocket(unsigned short port, const char* host);

    int Send(const char* buffer, int length)
    {
      return -1;
    }

    int Receive(char* buffer, int length)
    {
      // TODO
      return -1;
    }
  };

  using TcpSocket = StreamSocket<Protocol::TCP>;

  template<Protocol pr>
  SocketHandle StreamSocket<pr>::CreateServerSocket(unsigned short port)
  {
    SocketHandle sockfd;
    addrinfo *res = nullptr, *p = nullptr;
    ProtocolData prot(pr);
    
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

  template <Protocol pr>
  SocketHandle StreamSocket<pr>::CreateClientSocket(unsigned short port, const char* host)
  {
    addrinfo *res = nullptr, *p = nullptr;
    SocketHandle sockfd;
    ProtocolData prot(pr);
    
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
