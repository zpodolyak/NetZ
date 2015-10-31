#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "resource_manager.h"
#include "http_connection.h"
#include "http_server.h"

namespace NetZ
{
namespace Http
{
  HttpServer::HttpServer(const ConnectionData& conn)
    : reactor()
    , svrSocket(&reactor, ProtocolData<Protocol::TCP>(), conn)
    , clientSocket(&reactor)
    , resource_mgr()
  {
    svrSocket.SetNonBlocking(true);
    svrSocket.SetSocketOption(SocketOption (SOL_SOCKET, SO_REUSEADDR, 1));
    StartAccepting();
  }

  void HttpServer::Start()
  {
    while (reactor.IsRunning())
      reactor.Run();
  }

  void HttpServer::StartAccepting()
  {
    auto conn = svrSocket.LocalConnection();
    svrSocket.Accept(clientSocket, &conn, [this](const std::error_code& ec)
    {
      if (!ec)
      {
        DebugMessage("received new connection!");
        connections.insert(make_unique<HttpConnection>(std::move(clientSocket), &resource_mgr));
        StartAccepting();
      }
    });
  }
}
}
