#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "resource_manager.h"
#include "http_connection.h"

namespace NetZ
{
namespace Http
{
  HttpConnection::HttpConnection(TcpSocket _socket, ResourceManager* rMgr)
    : socket(std::move(_socket))
    , resource_mgr(rMgr)
  {
    Read();
  }

  void HttpConnection::Read()
  {
  }

  void HttpConnection::Write()
  {
  }
}
}
