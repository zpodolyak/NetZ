#ifndef SOCKET_PLATFORM_H
#define SOCKET_PLATFORM_H

namespace NetZ
{
  namespace SocketPlatform
  {
    void InitPlatform();
    void ShutdownPlatform();
    void SetNonBlocking(SocketHandle socket, bool mode);
    void Close(SocketHandle socket);
    const char* inet_ntop(int af, const void* src, char* dest, size_t length);
    int inet_pton(int af, const char* src, void* dest);
  }
}

#endif
