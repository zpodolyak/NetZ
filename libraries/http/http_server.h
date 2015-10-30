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

    HttpServer(const ConnectionData& conn);
    
    void Start();
  private:
    void StartAccepting();

    Reactor reactor;
    TcpServerSocket svrSocket;
    TcpSocket clientSocket;
    ResourceManager resource_mgr;
    std::set<std::unique_ptr<HttpConnection>> connections;
  };
}
}