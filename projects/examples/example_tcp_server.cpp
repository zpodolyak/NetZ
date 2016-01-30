#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "libraries/network/include/timer.h"
#include "example_helper.h"

using NetZ::Protocol;
using NetZ::ProtocolData;
using NetZ::SocketService;
using NetZ::ConnectionData;
using NetZ::Address;
using NetZ::TcpSocket;
using NetZ::TcpServerSocket;

char dataBuffer[1024];
SocketService service;
TcpSocket client(&service);

int main(int argc, char* argv[])
{
  NetZ::SocketPlatform::InitPlatform();
  ProtocolData<Protocol::TCP> protocol;

  ConnectionData conn;
  TcpServerSocket server(&service, protocol, conn);
  server.SetNonBlocking(true);
  conn = server.LocalConnection();
  DebugMessage("waiting for connections on port %" PRIu16, conn.GetPort());
  server.Accept(client, conn, [](const std::error_code& ec)
  {
    if (!ec)
    {
      DebugMessage("received new connection!");
      client.Receive(dataBuffer, 128, 0, [](int bytes_transferred, const std::error_code& ec)
      {
        if (bytes_transferred > 0 && !ec)
        {
          DebugMessage("successfully received %d bytes", bytes_transferred);
          SampleRecord rec;
          rec.Load(dataBuffer);
          rec.DumpRecord();
          service.AddTimer(NetZ::Util::Timer(5000, 5000, []() { service.Stop(); }));
        }
      });
    }
  });
  while (service.IsRunning())
    service.Run();

  NetZ::SocketPlatform::ShutdownPlatform();
  return 0;
}
