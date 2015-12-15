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

    TcpServerSocket svrSocket;
    TcpSocket clientSocket;
    ResourceManager resource_mgr;
    SocketService service;
    std::set<std::unique_ptr<HttpConnection>> connections;
  };
}
}