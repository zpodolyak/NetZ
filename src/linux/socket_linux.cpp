#include "common.h"

namespace Netz
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
      int flags = ::fcntl(socket, F_GETFL, 0);

      if (flags < 0 || ::fcntl(socket, F_SETFL, flags | O_NONBLOCK) < 0)
        PrintError("fcntl failed");
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
