#include "common.h"
#include "example_helper.h"

using Netz::Protocol;
using Netz::ProtocolData;
using Netz::Reactor;
using Netz::ConnectionData;
using Netz::Address;
using Netz::TcpSocket;
using Netz::TcpServerSocket;

char dataBuffer[1024];
Reactor rtor;
TcpSocket client(&rtor);

void OnReceive(int bytes_transferred, const std::error_code& ec)
{
  if (bytes_transferred > 0 && !ec)
  {
    DebugMessage("successfully received %d bytes", bytes_transferred);
    SampleRecord rec;
    rec.Load(dataBuffer);
    rec.DumpRecord();
    rtor.Stop();
  }
}

int main(int argc, char* argv[])
{
  Netz::SocketPlatform::InitPlatform();
  ProtocolData<Protocol::TCP> protocol;

  ConnectionData conn;
  TcpServerSocket server(&rtor, protocol, conn);
  server.SetNonBlocking(true);
  conn = server.LocalConnection();
  DebugMessage("waiting for connections on port %" PRIu16, conn.GetPort());
  server.Accept(client, &conn, [&](TcpSocket& client, const std::error_code& ec)
  {
    if (!ec)
    {
      DebugMessage("received new connection!");
      client.Receive(dataBuffer, 128, 0, OnReceive);
    }
  });
  while (rtor.IsRunning())
    rtor.Run(200);

  Netz::SocketPlatform::ShutdownPlatform();
  return 0;
}
