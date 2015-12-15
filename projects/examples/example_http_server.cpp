#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "libraries/http/http_common.h"
#include "libraries/http/http_server.h"

int main(int argc, char* argv[])
{
  NetZ::SocketPlatform::InitPlatform();
  NetZ::ConnectionData conn{NetZ::Address::FromString("127.0.0.1"), 1112};
  NetZ::Http::HttpServer server{conn, "."};
  server.Start();
  NetZ::SocketPlatform::ShutdownPlatform();
  return 0;
}