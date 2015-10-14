#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "http_connection.h"
#include "http_server.h"

namespace NetZ
{
namespace Http
{
  HttpServer::HttpServer(SocketService* service, const ConnectionData& conn)
    : svrSocket(service, ProtocolData<Protocol::TCP>(), conn)
  {
  }

  void HttpServer::Start()
  {
  }
}
}
