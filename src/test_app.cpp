#include "common.h"

using Netz::Protocol;
using Netz::ProtocolData;

int main(int argc, char* argv[])
{
  Netz::SocketPlatform::InitPlatform();
  SocketHandle s;
  if(argc == 3)
  {
    if((s = Netz::TcpSocket::CreateClientSocket<ProtocolData<Protocol::TCP>>(std::stoi(argv[2]), argv[1])) != INVALID_SOCKET)
      DebugMessage("connected to server!");
    else
      DebugMessage("could not obtain socket! Exiting...");
  }
  else
  {
    struct sockaddr_storage addr;
    if((s = Netz::TcpSocket::CreateServerSocket<ProtocolData<Protocol::TCP>>(1112)) != INVALID_SOCKET)
    {
      DebugMessage("waiting for connections...");
      
      // bypass the Reactor for now
      for(;;)
      {
#ifdef WIN32
        auto sockLen = int(sizeof(sockaddr_storage));
#else
        auto sockLen = sizeof(sockaddr_storage);
#endif
        int in = ::accept(s, (struct sockaddr *)&addr, &sockLen);
        if (in == -1) 
        {
          PrintError("accept");
          continue;
        }           
        DebugMessage("received new connection!");
        Netz::SocketPlatform::Close(in);
        break;                              
      }
    }
    else
      DebugMessage("could not obtain socket! Exiting...");
 }
  Netz::SocketPlatform::ShutdownPlatform();
	return 0;
}
