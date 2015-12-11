#pragma once

#include "libraries/network/include/socket_service.h"

namespace NetZ
{
namespace Http
{
  class HttpConnection;

  class HttpServer
  {
  public:
    HttpServer(const HttpServer&) = delete;
    HttpServer& operator=(const HttpServer&) = delete;

    HttpServer(const ConnectionData& conn, const std::string& documentRoot);
    
    void Start();
    void RemoveConnection(HttpConnection* conn);
  private:
    void StartAccepting();

    TcpServerSocket svrSocket;
    ResourceManager resource_mgr;
    SocketService service;
    std::set<std::unique_ptr<HttpConnection>> connections;
  };
}
}