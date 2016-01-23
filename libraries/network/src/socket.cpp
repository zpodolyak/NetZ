#include "libraries/common.h"
#include "network.h"

#ifdef WIN32
# pragma comment(lib, "ws2_32.lib")
#endif

namespace NetZ
{
  bool SocketOption::operator==(const SocketOption& other) const
  {
    return level == other.level && name == other.name && value == other.value;
  }

  SocketBase::SocketBase()
  : socket(INVALID_SOCKET)
  {
  }

  SocketBase::~SocketBase()
  {
    if (IsOpen() && service)
    {
      service->CancelDescriptor(socket);
      Close();
    }
  }

  SocketBase::SocketBase(SocketService* _service)
    : socket(INVALID_SOCKET)
    , service(_service)
  {
  }

  bool SocketBase::IsOpen() const
  {
    return socket != INVALID_SOCKET;
  }

  std::error_code SocketBase::Assign(SocketHandle _socket)
  {
    if (IsOpen())
      return std::make_error_code(std::errc::already_connected);
    socket = _socket;
    if (service)
      service->RegisterDescriptor(socket);
    return std::error_code();
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

  SocketHandle SocketBase::Handle() const
  {
    return socket;
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
#ifdef WIN32
    auto optLen = int(opt.Size());
#else
    auto optLen = opt.Size();
#endif
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
    return isNonBlocking;
  }

  void SocketBase::SetNonBlocking(bool mode)
  {
    SocketPlatform::SetNonBlocking(socket, mode);
    isNonBlocking = mode;
  }
}
