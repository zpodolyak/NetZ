#pragma once

#include "libraries/network/include/socket_service.h"
#include "http_connection.h"
#include "resource_manager.h"

namespace NetZ
{
namespace Http
{
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

    SocketService service;
    TcpServerSocket svrSocket;
    ResourceManager resource_mgr;
    std::vector<std::unique_ptr<HttpConnection>> connections;
  };
}
}
