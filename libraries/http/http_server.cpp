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
        conn = connections.erase(conn);
      }
      else ++conn;
    }
    if (connections.empty())
      StartAccepting();
  }

  void HttpServer::StartAccepting()
  {
    connections.push_back(make_unique<HttpConnection>(&service, &resource_mgr));
    svrSocket.Accept(connections.back()->socket, svrSocket.LocalConnection(), [this](const std::error_code& ec)
    {
      auto conn = connections.back().get();
      conn->socketTimeoutTimer = service.AddTimer(Util::Timer(socketTimeoutDuration, socketTimeoutDuration, [this, conn]() { RemoveConnection(conn); }));
      if (!ec)
      {
        DebugMessage("received new HTTP connection! %d", conn->socket.Handle());
        conn->Start();
      }
      StartAccepting();
    });
  }
}
}

