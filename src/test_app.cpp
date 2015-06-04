#include "common.h"

int main(int argc, char* argv[])
{
  SocketHandle s;
  if(argc == 3)
  {
    if((s = Netz::TcpSocket::CreateClientSocket<Netz::ProtocolData<Netz::Protocol::TCP>>(std::stoi(argv[2]), argv[1])) != INVALID_SOCKET)
    {
      DebugMessage("connected to server at %s", argv[1]);
    }
    else
      DebugMessage("could not obtain socket! Exiting...");
  }
  else
  {
    struct sockaddr_storage addr;
    if((s = Netz::TcpSocket::CreateServerSocket<Netz::ProtocolData<Netz::Protocol::TCP>>(1112)) != INVALID_SOCKET)
    {
      DebugMessage("waiting for connections...");
      
      // bypass the Reactor for now
      for(;;)
      {
        size_t sockLen = sizeof(sockaddr_storage);
        int in = ::accept(s, (struct sockaddr *)&addr, &sockLen);
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
