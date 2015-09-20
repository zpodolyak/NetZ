#include "common.h"
#include "example_helper.h"

using Netz::Protocol;
using Netz::ProtocolData;
using Netz::Reactor;
using Netz::ConnectionData;
using Netz::Address;
using Netz::TcpSocket;

Reactor rtor;
bool connectionReady = false;
char dataBuffer[1024];

void ConnectionEstablished(const std::error_code& ec)
{
  if (ec)
    DebugMessage("could not obtain socket! Exiting...");
  else
  {
    DebugMessage("connected to server!");
    connectionReady = true;
  }
}

void OnSend(int bytes_transferred, const std::error_code& ec)
{
  if (bytes_transferred > 0 && !ec)
  {
    DebugMessage("successfully sent %d bytes", bytes_transferred);
  }
}

int main(int argc, char* argv[])
{
  Netz::SocketPlatform::InitPlatform();
  ProtocolData<Protocol::TCP> protocol;
  TcpSocket client(&rtor, protocol);

  if (argc == 3)
  {
    ConnectionData conn(Address::FromString(argv[1]), std::stoi(argv[2]));
    client.SetNonBlocking(true);
    client.Connect(conn, ConnectionEstablished);
  }
  else
  {
    DebugMessage("usage: example_client <host_ip> <port>");
  }
  while (rtor.IsRunning())
  {
    rtor.Run(200);
    if (connectionReady)
    {
      SampleRecord smpl(1112, "Netz Sample Send");
      smpl.Save(dataBuffer);
      client.Send(dataBuffer, smpl.MessageLength(), 0, OnSend);
      connectionReady = false;
    }
  }

  Netz::SocketPlatform::ShutdownPlatform();
  return 0;
}
