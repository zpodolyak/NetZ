#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "libraries/network/include/timer.h"
#include "example_helper.h"

using NetZ::Protocol;
using NetZ::ProtocolData;
using NetZ::SocketService;
using NetZ::ConnectionData;
using NetZ::Address;
using NetZ::UdpSocket;

char dataBuffer[1024];
SocketService service;

int main(int argc, char* argv[])
{
  NetZ::SocketPlatform::InitPlatform();
  ProtocolData<Protocol::UDP> protocol;

  UdpSocket server(&service, protocol);
  ConnectionData conn;
  server.Bind(conn);
  server.SetNonBlocking(true);
  conn = server.LocalConnection();
  DebugMessage("waiting to receive from a new connection on port %" PRIu16, conn.GetPort());
  server.ReceiveFrom(dataBuffer, 128, 0, conn, [](int bytes_transferred, const std::error_code& ec)
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
  while (service.IsRunning())
    service.Run();

  NetZ::SocketPlatform::ShutdownPlatform();
  return 0;
}
