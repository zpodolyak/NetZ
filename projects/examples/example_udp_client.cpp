#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "example_helper.h"

using NetZ::Protocol;
using NetZ::ProtocolData;
using NetZ::SocketService;
using NetZ::ConnectionData;
using NetZ::Address;
using NetZ::UdpSocket;

SocketService service;
bool connectionReady = false;
char dataBuffer[1024];

int main(int argc, char* argv[])
{
  NetZ::SocketPlatform::InitPlatform();
  ProtocolData<Protocol::UDP> protocol;
  UdpSocket client(&service, protocol);
  ConnectionData conn;

  if (argc == 3)
  {
    conn.SetData(Address::FromString(argv[1]));
    conn.SetPort(std::stoi(argv[2]));
    client.SetNonBlocking(true);
  }
  else
  {
    DebugMessage("usage: example_client <host_ip> <port>");
  }
  while (service.IsRunning())
  {
    service.Run();
    SampleRecord smpl(1112, "NetZ Sample SendTo");
    smpl.Save(dataBuffer);
    client.SendTo(dataBuffer, smpl.MessageLength(), 0, conn, [](int bytes_transferred, const std::error_code& ec)
    {
      if (bytes_transferred > 0 && !ec)
      {
        DebugMessage("successfully sent %d bytes", bytes_transferred);
      }
      service.Stop();
    });
  }
  NetZ::SocketPlatform::ShutdownPlatform();
  return 0;
}
