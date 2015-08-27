#include "common.h"

using Netz::Protocol;
using Netz::ProtocolData;
using Netz::Reactor;
using Netz::ConnectionData;
using Netz::Address;
using Netz::TcpSocket;
using Netz::TcpServerSocket;

int main(int argc, char* argv[])
{
  Netz::SocketPlatform::InitPlatform();
  ProtocolData<Protocol::TCP> protocol;

  if(argc == 3)
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
    ConnectionData conn(Address::FromString("127.0.0.1"), 1112);
    TcpServerSocket server(protocol, conn);
    TcpSocket client;
    DebugMessage("waiting for connections...");
    server.Accept(client, &conn);
    DebugMessage("received new connection!");
  }
 Netz::SocketPlatform::ShutdownPlatform();
 return 0;
}
