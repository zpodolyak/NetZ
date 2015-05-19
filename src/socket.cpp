#include "common.h"

namespace Netz
{
  Socket::Socket()
  {
  }

  std::shared_ptr<Socket> Socket::CreateServerSocket(int type, uint16_t port)
  {
    std::shared_ptr<Socket> s(new Socket());
    addrinfo *res = nullptr, *p = nullptr;
    
    if (Address::ResolveFromHostname(nullptr, type, std::to_string(port).c_str(), &res))
    {
      for (p=res; p; p=p->ai_next)
      {
        s->socket = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s->socket < 0)  
            continue;
        
        int yes=1;
        ::setsockopt(s->socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
      
        if (::bind(s->socket, p->ai_addr, p->ai_addrlen) < 0) 
        {
          ::close(s->socket);
          continue;
        }
        break;
      }
      ::freeaddrinfo(res);
      if(!p)
      {
        fprintf(stderr, "failed to bind socket!\n");
        s.reset();
      }
    }
    else
      s.reset();
    return s;
  }

  std::shared_ptr<Socket> Socket::CreateClientSocket(int type, uint16_t port, const char* host)
  {
    std::shared_ptr<Socket> s(new Socket());
    addrinfo *res = nullptr, *p = nullptr;
    
    if (Address::ResolveFromHostname(host, type, std::to_string(port).c_str(), &res))
    {
      for (p=res; p; p=p->ai_next)
      {
        if ((s->socket = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
          continue;
        
        if((::connect(s->socket, p->ai_addr, p->ai_addrlen)) < 0)
        {
          PrintError("connect() error");
          ::close(s->socket);
          continue;
        }
        break;
      }
      ::freeaddrinfo(res);
      if(!p)
      {
        fprintf(stderr, "failed to connect to server\n");
        s.reset();
      }
    }
    else
      s.reset();
    
    return s;
  }

  void Socket::SetNonBlocking()
  {
    int flags = ::fcntl(socket, F_GETFL, 0);

    if (flags < 0 || ::fcntl(socket, F_SETFL, flags | O_NONBLOCK) < 0)
      PrintError("fcntl failed");
  }

}
