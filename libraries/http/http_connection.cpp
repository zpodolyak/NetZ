#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "http_connection.h"

namespace NetZ
{
namespace Http
{
  HttpConnection::HttpConnection(TcpSocket _socket)
    : socket(std::move(_socket))
  {
  }
}
}
