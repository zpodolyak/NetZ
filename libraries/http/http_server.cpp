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
    auto conn = svrSocket.LocalConnection();
    TcpSocket clientSocket(&service);
    svrSocket.Accept(clientSocket, &conn, [this, &clientSocket](const std::error_code& ec)
    {
      if (!ec)
      {
        DebugMessage("received new HTTP connection!");
        auto newConn = make_unique<HttpConnection>(std::move(clientSocket), &service, &resource_mgr);
        newConn->socketTimeoutTimer = service.AddTimer(Util::Timer(0, socketTimeoutDuration, [this, &newConn]() { RemoveConnection(newConn.get()); }));
        connections.insert(std::move(newConn));
        newConn->Start();
        StartAccepting();
      }
    });
  }
}
}
