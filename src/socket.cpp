#include "common.h"

namespace Netz
{
  bool SocketOption::operator==(const SocketOption& other) const
  {
    return level == other.level && name == other.name && value == other.value;
  }

  SocketBase::SocketBase()
  : socket(INVALID_SOCKET)
  {
  }

  bool SocketBase::IsOpen() const
  {
    return socket != INVALID_SOCKET;
  }

  void SocketBase::Bind(const ConnectionData& conn)
  {
    if(socket == INVALID_SOCKET)
      return;
    
    if (::bind(socket, (sockaddr*)&conn.data, sizeof(sockaddr_in)) < 0)
      PrintError("SocketBase::Bind"); 
  }

  int SocketBase::Connect(const ConnectionData& conn)
  {
    std::error_code ec;
    const sockaddr_in *socketAddress = &conn.data;

    if (socket == INVALID_SOCKET || !socketAddress)
      return SOCKET_ERROR;
          
    return ErrorWrapper(::connect(socket,(const sockaddr*)socketAddress, sizeof(sockaddr_in)), ec); 
  }

  void SocketBase::Close()
  {
    SocketPlatform::Close(socket);
    socket = INVALID_SOCKET;
  }

  std::error_code SocketBase::GetSocketOption(SocketOption& opt) const
  {
    if (socket == INVALID_SOCKET)
      return std::make_error_code(std::errc::bad_file_descriptor);
    std::error_code ec;
    auto optLen = opt.Size();
    ErrorWrapper(::getsockopt(socket, opt.level, opt.name, (char*)opt.ValueData(), &optLen), ec);
    return ec;
  }

  std::error_code SocketBase::SetSocketOption(const SocketOption& opt)
  {
    if (socket == INVALID_SOCKET)
      return std::make_error_code(std::errc::bad_file_descriptor);
    std::error_code ec;
    ErrorWrapper(::setsockopt(socket, opt.level, opt.name, (const char*)opt.ValueData(), (int)opt.Size()), ec);
    return ec;
    
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
