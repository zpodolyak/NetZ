#pragma once

namespace NetZ
{
namespace Http
{
  class HttpServer
  {
  public:
    HttpServer(const HttpServer&) = delete;
    HttpServer& operator=(const HttpServer&) = delete;

    HttpServer(SocketService* service, const ConnectionData& conn);
    
    void Start();
  private:
    TcpServerSocket svrSocket;
    std::set<std::unique_ptr<HttpConnection>> connections;
  };
}
}