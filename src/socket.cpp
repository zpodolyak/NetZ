#include "common.h"

namespace Netz
{
  Socket::Socket(Mode mode)
  : m(mode)
  {
  }

  std::shared_ptr<Socket> Socket::CreateServerSocket(int type, uint16_t port)
  {
    std::shared_ptr<Socket> s(new Socket(Mode::Server));
    addrinfo *res = nullptr, *p = nullptr;
    Address addr{ std::string() };
    
    if (addr.GetAddressInfo(type, std::to_string(port).c_str(), &res))
    {
      for (p=res; p; p=p->ai_next)
      {
        s->socket = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s->socket < 0)  
            continue;
        
        int yes=1;
        setsockopt(s->socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
      
        if (bind(s->socket, p->ai_addr, p->ai_addrlen) < 0) 
        {
          ::close(s->socket);
          continue;
        }
        break;
      }
      freeaddrinfo(res);
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
    std::shared_ptr<Socket> s(new Socket(Mode::Client));
    addrinfo *res = nullptr, *p = nullptr;
    Address addr{ host };
    
    if (addr.GetAddressInfo(type, std::to_string(port).c_str(), &res))
    {
      for (p=res; p; p=p->ai_next)
      {
        if ((s->socket = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
          continue;
        
        if((connect(s->socket, p->ai_addr, p->ai_addrlen)) < 0)
        {
          PrintError("connect() error");
          ::close(s->socket);
          continue;
        }
        break;
      }
      freeaddrinfo(res);
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
    int flags = fcntl(socket, F_GETFL, 0);

    if (flags < 0 || fcntl(socket, F_SETFL, flags | O_NONBLOCK) < 0)
      PrintError("fcntl failed");
  }

  int Socket::Read(uint8_t *buffer, int length)
  {
    int nbytes = ::recv(socket, buffer, length,0);

    if(nbytes==-1)
    {
      PrintError("recv read error");
      return -1;
    }
    else if(nbytes==0)
    {
      PrintError("Remote host closed the connection.");
      return 0;
    }
    return nbytes;	
  }

  int Socket::Write(uint8_t *data, int length)
  {
    int nbytes=0;
    if((nbytes = ::send(socket, data, length, 0))== -1)
    {
      PrintError("send error");
      return -1;
    }
    return nbytes;
  }
}
