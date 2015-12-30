#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "http_server.h"

namespace NetZ
{
namespace Http
{
  HttpServer::HttpServer(const ConnectionData& conn, const std::string& documentRoot)
    : svrSocket(&service, ProtocolData<Protocol::TCP>(), conn)
    , clientSocket(&service)
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
    svrSocket.Accept(clientSocket, svrSocket.LocalConnection(), [this](const std::error_code& ec)
    {
      if (!ec)
      {
        DebugMessage("received new HTTP connection!");
        auto it = connections.insert(make_unique<HttpConnection>(std::move(clientSocket), &service, &resource_mgr));
        auto newConn = it.first->get();
        newConn->socketTimeoutTimer = service.AddTimer(Util::Timer(socketTimeoutDuration, socketTimeoutDuration, [this, newConn]() { RemoveConnection(newConn); }));
        newConn->Start();

        StartAccepting();
      }
    });
  }
}
}
