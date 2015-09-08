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

  ConnectionData conn;
  TcpServerSocket server(&rtor, protocol, conn);
  server.SetNonBlocking(true);
  conn = server.LocalConnection();
  TcpSocket client;
  DebugMessage("waiting for connections on port %" PRIu16, conn.GetPort());
  server.Accept(client, &conn, [&](TcpSocket& client, const std::error_code& ec)
  {
    DebugMessage("received new connection!");
    rtor.Stop();
  });
  while (rtor.IsRunning())
    rtor.Run(200);

  Netz::SocketPlatform::ShutdownPlatform();
  return 0;
}
