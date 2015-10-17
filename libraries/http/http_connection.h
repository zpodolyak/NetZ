#pragma once

#include "http_common.h"
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

    HttpConnection(TcpSocket _socket);
  private:
    TcpSocket socket;
    HttpMessageRequest request;
    HttpMessageResponse response;
  };
}
}