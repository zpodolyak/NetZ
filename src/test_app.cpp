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
  Reactor rtor;

  if (argc == 3)
  {
    TcpSocket client(&rtor, protocol);
    ConnectionData conn(Address::FromString(argv[1]), std::stoi(argv[2]));
    client.Connect(conn, [](const std::error_code& ec)
    {
      if (ec != std::errc::operation_would_block)
        DebugMessage("could not obtain socket! Exiting...");
      else
        DebugMessage("connected to server!");
    });
  }
  else
  {
    ConnectionData conn;
    TcpServerSocket server(&rtor, protocol, conn);
    server.SetNonBlocking(true);
    conn = server.LocalConnection();
    TcpSocket client;
    DebugMessage("waiting for connections on port %h", conn.GetPort());
    server.Accept(client, &conn, [](TcpSocket& client, const std::error_code& ec)
    {
      DebugMessage("received new connection!");
    });
  }
  while (rtor.IsRunning()) 
    rtor.Run(200);
  Netz::SocketPlatform::ShutdownPlatform();
  return 0;
}
