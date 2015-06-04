#include "common.h"

namespace Netz
{
  SocketBase::SocketBase()
  : socket(INVALID_SOCKET)
  {
  }

  void SocketBase::Bind(const ConnectionData& conn)
  {
    if(socket == INVALID_SOCKET)
      PrintError("SocketBase::Bind");
    
    if (::bind(socket, (sockaddr*)&conn.data, sizeof(sockaddr_in)) < 0)
      PrintError("SocketBase::Bind"); 
  }

  void SocketBase::Connect(const ConnectionData& conn)
  {
  }

  void SocketBase::Close()
  {
    SocketPlatform::Close(socket);
    socket = INVALID_SOCKET;
  }

  void SocketBase::GetSocketOption(SocketOption& opt) const
  {
  }

  void SocketBase::SetSocketOption(const SocketOption& opt)
  {
  }

  ConnectionData SocketBase::LocalConnection() const
  {
    ConnectionData cd;
    int len = int(cd.Size());
    if (::getsockname(socket, (sockaddr*)&cd.data, (socklen_t*)&len))
      PrintError("SocketBase::LocalConnection");
    return cd;
  }
  
  ConnectionData SocketBase::RemoteConnection() const
  {
    ConnectionData cd;
    int len = int(cd.Size());
    if (::getpeername(socket, (sockaddr*)&cd.data, (socklen_t*)&len))
      PrintError("SocketBase::RemoteConnection");
    return cd;
  }

  bool SocketBase::IsNonBlocking() const
  {
    return false;
  }

  void SocketBase::SetNonBlocking(bool mode)
  {
    SocketPlatform::SetNonBlocking(socket, mode);
  }
}
