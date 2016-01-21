#include "libraries/common.h"

namespace NetZ
{
  namespace SocketPlatform
  {
    void InitPlatform()
    {
    }

    void ShutdownPlatform()
    {
    }

    void SetNonBlocking(SocketHandle socket, bool mode)
    {
      u_long arg = (mode ? 1 : 0);
      std::error_code ec;
      if (ErrorWrapper(::ioctl(socket, FIONBIO, &arg), ec))
        PrintError(ec);
    }

    void Close(SocketHandle socket)
    {
      ::close(socket);
    }

    const char* inet_ntop(int af, const void* src, char* dest, size_t length)
    {
      return ::inet_ntop(af, src, dest, length);
    }

    int inet_pton(int af, const char* src, void* dest)
    {
      return ::inet_pton(af, src, dest);
    }
  }
}
