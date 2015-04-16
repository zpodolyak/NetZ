#include "common.h"

namespace Netz
{
  Socket::Socket(Reactor &_reactor, int type, unsigned short port)
  : reactor(_reactor)
  , serverType(type)
  , serverPort(port)	
  , socket_fd(CreateServerSocket())
  {
    SetSocketOptions();
    reactor.AddEventHandler(new Event(socket_fd), std::bind(&Socket::OnIncomingConnection,this, std::placeholders::_1));
  }

  void Socket::OnIncomingConnection(int fd)
  {
    assert(fd == socket_fd);

    Address client;
    if(int newConn = Accept(client))
      printf("incoming connection received fd=%d\n", newConn);
  }

  void Socket::OnSocketEvent(int /*fd*/)
  {
    // TODO: handle socket event here
  }

  void Socket::Listen()
  {
    listen(socket_fd, 10);	
  }

  int Socket::Accept(Address &address)
  {
    int in;
    struct sockaddr_in client;
    socklen_t client_size = sizeof(sockaddr_in);

    in=accept(socket_fd,(struct sockaddr*)&client, &client_size);
    
    if(in==-1)
    {
      PrintError("accept error");
      return 0;
    }
    
    address.SetResolvedAddress(client);
    reactor.AddEventHandler(new Event(in), std::bind(&Socket::OnSocketEvent, this, std::placeholders::_1));
    return in;
  }

  int Socket::CreateServerSocket()
  {
    struct addrinfo hints, *res, *p;
    int status, sfd;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = serverType;
    hints.ai_flags = AI_PASSIVE;
    
    if ((status = getaddrinfo(nullptr, to_string(serverPort).c_str(), &hints, &res)) != 0)
    {	
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
      return -1;
    }
    
    for(p=res; p!=nullptr; p=p->ai_next)
    {
      sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
      if (sfd < 0)  
          continue;
      
      int yes=1;
      setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    
      if (bind(sfd, p->ai_addr, p->ai_addrlen) < 0) 
      {
        close(sfd);
        continue;
      }
      break;
    }
    freeaddrinfo(res);
    if(p==nullptr)
    {
      PrintError("Failed to bind socket!");
      return -1;
    }
    return sfd;
  }

  void Socket::SetSocketOptions()
  {
    int flags, s;
    flags = fcntl(socket_fd, F_GETFL, 0);
    if(flags < 0)
    {
      PrintError("fcntl failed");
      return;
    }

    flags |= O_NONBLOCK;
    s = fcntl(socket_fd, F_SETFL, flags);
    if(s<0)
    {
      PrintError("fcntl failed");
      return;
    }
  }

  int Socket::ReadStream(char *buffer, int buff_length)
  {
    int nbytes=recv(socket_fd, buffer, buff_length,0);

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

  int Socket::WriteStream(const char *data, int length)
  {
    int nbytes=0;
    if((nbytes=send(socket_fd, data, length, 0))== -1)
    {
      PrintError("send error");
      return -1;
    }
    return nbytes;
  }
}
