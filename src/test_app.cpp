#include "common.h"

int main(int argc, char* argv[])
{
  if(argc == 3)
  {
    auto s = Netz::Socket::CreateClientSocket(SOCK_STREAM, 1112, argv[1]);

    if(s)
    {
      DebugMessage("connected to server at %s", argv[1]);
    }
    else
      DebugMessage("could not obtain socket! Exiting...");
  }
  else
  {
    struct sockaddr_storage addr;
    auto s = Netz::Socket::CreateServerSocket(SOCK_STREAM, 1112);

    if(s)
    {
      ::listen(s->socket, 10);
      DebugMessage("waiting for connections...");
      
      // bypass the Reactor for now
      for(;;)
      {
        size_t sockLen = sizeof(sockaddr_storage);
        int in = ::accept(s->socket, (struct sockaddr *)&addr, &sockLen);
        if (in == -1) 
        {
          PrintError("accept");
          continue;
        }           
        DebugMessage("received new connection!");
        ::close(in);
        break;                              
      }
    }
    else
      DebugMessage("could not obtain socket! Exiting...");
 }

	return 0;
}
