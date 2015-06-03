#ifndef SOCKET_LINUX_H
#define SOCKET_LINUX_H

namespace Netz
{
  namespace SocketPlatform
  {
    void SetNonBlocking(SocketHandle socket, bool mode);
    void Close(SocketHandle socket);
    const char* inet_ntop(int af, const void* src, char* dest, size_t length);
    int inet_pton(int af, const char* src, void* dest);
  }
}

#endif
