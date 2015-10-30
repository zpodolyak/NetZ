#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "example_helper.h"

using NetZ::Protocol;
using NetZ::ProtocolData;
using NetZ::Reactor;
using NetZ::ConnectionData;
using NetZ::Address;
using NetZ::TcpSocket;

Reactor rtor;
bool connectionReady = false;
char dataBuffer[1024];

int main(int argc, char* argv[])
{
  NetZ::SocketPlatform::InitPlatform();
  ProtocolData<Protocol::TCP> protocol;
  TcpSocket client(&rtor, protocol);

  if (argc == 3)
  {
    ConnectionData conn(Address::FromString(argv[1]), std::stoi(argv[2]));
    client.SetNonBlocking(true);
    client.Connect(conn, [](const std::error_code& ec)
    {
      if (ec)
        DebugMessage("could not obtain socket! Exiting...");
      else
      {
        DebugMessage("connected to server!");
        connectionReady = true;
      }
    });
  }
  else
  {
    DebugMessage("usage: example_client <host_ip> <port>");
  }
  while (rtor.IsRunning())
  {
    rtor.Run();
    if (connectionReady)
    {
      SampleRecord smpl(1112, "NetZ Sample Send");
      smpl.Save(dataBuffer);
      client.Send(dataBuffer, smpl.MessageLength(), 0, [&](int bytes_transferred, const std::error_code& ec)
      {
        if (bytes_transferred > 0 && !ec)
        {
          DebugMessage("successfully sent %d bytes", bytes_transferred);
        }
        rtor.Stop();
      });
      connectionReady = false;
    }
  }
  NetZ::SocketPlatform::ShutdownPlatform();
  return 0;
}
