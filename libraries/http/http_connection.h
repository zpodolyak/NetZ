#pragma once

#include "http_common.h"
#include "http_server.h"
#include "http_parser.h"

namespace NetZ
{
namespace Http
{
  class HttpConnection
  {
  public:
    HttpConnection(const HttpConnection&) = delete;
    HttpConnection& operator=(const HttpConnection&) = delete;

    HttpConnection(HttpServer* _server, TcpSocket _socket, ResourceManager* rMgr);
    ~HttpConnection() { Stop(); }
    void Stop() { socket.Close(); }
  private:
    TcpSocket socket;
    HttpServer* server;
    HttpMessageRequest request;
    HttpMessageResponse response;
    ResourceManager* resource_mgr;
    Util::Timer socketTimeout;
    void Read(HttpParser::ParseState state);
    void Write(const InputBuffer& buffer);
    void WriteDefaultResponse();
  };
}
}
