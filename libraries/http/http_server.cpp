#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "http_server.h"

namespace NetZ
{
namespace Http
{
  HttpServer::HttpServer(const ConnectionData& conn, const std::string& documentRoot)
    : service()
    , svrSocket(&service, ProtocolData<Protocol::TCP>(), conn)
    , resource_mgr(documentRoot)
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
    DebugMessage("removing connection %d", connection->socket.Handle());
    service.CancelDescriptor(connection->socket.Handle());
    service.CancelTimer(connection->socketTimeoutTimer);

    auto conn = std::begin(connections);
    while (conn != std::end(connections))
    {
      if ((*conn).get() == connection)
      {
        if (connection->socket.IsOpen()) connection->Stop();
        conn = connections.erase(conn);
      }
      else ++conn;
    }
  }

  void HttpServer::StartAccepting()
  {
    auto it = connections.insert(make_unique<HttpConnection>(&service, &resource_mgr));
    auto newConn = it.first->get();
    svrSocket.Accept(newConn->socket, svrSocket.LocalConnection(), [this, newConn](const std::error_code& ec)
    {
      newConn->socketTimeoutTimer = service.AddTimer(Util::Timer(socketTimeoutDuration, socketTimeoutDuration, [this, newConn]() { RemoveConnection(newConn); }));
      if (!ec)
      {
        DebugMessage("received new HTTP connection! %d", newConn->socket.Handle());
        newConn->Start();
      }
      StartAccepting();
    });
  }
}
}

