#include "common.h"

using Netz::Protocol;
using Netz::ProtocolData;
using Netz::ConnectionData;
using Netz::Address;
using Netz::TcpSocket;

int main(int argc, char* argv[])
{
  Netz::SocketPlatform::InitPlatform();
  ProtocolData<Protocol::TCP> protocol;

  if (argc == 3)
  {
    TcpSocket client(protocol);
    ConnectionData conn(Address::FromString(argv[1]), std::stoi(argv[2]));
    if(client.Connect(conn) != INVALID_SOCKET) 
      DebugMessage("connected to server!");
    else
      DebugMessage("could not obtain socket! Exiting...");
  }
  else
  {
    DebugMessage("usage: example_client <host_ip> <port>");
  }
  Netz::SocketPlatform::ShutdownPlatform();
  return 0;
}
