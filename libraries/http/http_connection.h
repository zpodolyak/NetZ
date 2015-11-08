#pragma once

#include "http_common.h"
#include "http_server.h"
#include "http_parser.h"

namespace NetZ
{
namespace Http
{
  constexpr uint64_t socketTimeoutDuration = 15000;

  class HttpConnection
  {
    friend class HttpServer;
  public:
    HttpConnection(const HttpConnection&) = delete;
    HttpConnection& operator=(const HttpConnection&) = delete;

    HttpConnection(TcpSocket&& _socket, ResourceManager* rMgr);
    ~HttpConnection() { Stop(); }
    void Stop();

  private:
    TcpSocket socket;
    HttpMessageRequest request;
    HttpMessageResponse response;
    ResourceManager* resource_mgr;
    Util::Timer* socketTimeout = nullptr;

    void Read(HttpParser::ParseState state);
    void Write(const InputBuffer& buffer);
    void WriteDefaultResponse();
  };
}
}
