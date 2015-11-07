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
    : svrSocket(&service, ProtocolData<Protocol::TCP>(), conn)
    , clientSocket(&service)
    , resource_mgr()
    , service()
  {
    svrSocket.SetNonBlocking(true);
    svrSocket.SetSocketOption(SocketOption (SOL_SOCKET, SO_REUSEADDR, 1));
    StartAccepting();
  }

  void HttpServer::Start()
  {
    while (service.IsRunning())
      service.Run();
  }

  void HttpServer::RemoveConnection(HttpConnection* connection)
  {
    auto conn = std::begin(connections);
    while (conn != std::end(connections))
    {
      if ((*conn).get() == connection)
        conn = connections.erase(conn);
      else
        ++conn;
    }
    connection->Stop();
  }

  void HttpServer::StartAccepting()
  {
    auto conn = svrSocket.LocalConnection();
    svrSocket.Accept(clientSocket, &conn, [this](const std::error_code& ec)
    {
      if (!ec)
      {
        DebugMessage("received new connection!");
        connections.insert(make_unique<HttpConnection>(this, std::move(clientSocket), &resource_mgr));
        StartAccepting();
      }
    });
  }
}
}
