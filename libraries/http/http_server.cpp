#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "http_server.h"

namespace NetZ
{
namespace Http
{
  HttpServer::HttpServer(const ConnectionData& conn, const std::string& documentRoot)
    : svrSocket(&service, ProtocolData<Protocol::TCP>(), conn)
    , resource_mgr(documentRoot)
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
    service.CancelTimer(connection->socketTimeoutTimer);

    auto conn = std::begin(connections);
    while (conn != std::end(connections))
    {
      if ((*conn).get() == connection)
        conn = connections.erase(conn);
      else
        ++conn;
    }
    if (connection->socket.IsOpen()) connection->Stop();
  }

  void HttpServer::StartAccepting()
  {
    auto it = connections.insert(make_unique<HttpConnection>(&service, &resource_mgr));
    auto newConn = it.first->get();
    newConn->socketTimeoutTimer = service.AddTimer(Util::Timer(socketTimeoutDuration, socketTimeoutDuration, [this, newConn]() { RemoveConnection( newConn ); }));

    svrSocket.Accept(newConn->Socket(), svrSocket.LocalConnection(), [this, newConn](const std::error_code& ec)
    {
      if (!ec)
      {
        DebugMessage("received new HTTP connection!");
        newConn->Start();
      }
      StartAccepting();
    });
  }
}
}
