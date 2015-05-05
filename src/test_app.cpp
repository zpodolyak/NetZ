#include "common.h"

int main(int argc, char* argv[])
{
  if(argc == 3)
  {
    auto s = Netz::Socket::CreateClientSocket(SOCK_STREAM, 1112, argv[1]);

    if(s)
    {
      printf("connected to server at %s\n", argv[1]);
    }
    else
      printf("could not obtain socket! Exiting...\n");
  }
  else
  {
    struct sockaddr_storage addr;
    auto s = Netz::Socket::CreateServerSocket(SOCK_STREAM, 1112);

    if(s)
    {
      ::listen(s->socket, 10);
      printf("waiting for connections...\n");
      
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
        printf("received new connection!\n");
        ::close(in);
        break;                              
      }
    }
    else
      printf("could not obtain socket! Exiting...\n");
 }

	return 0;
}
