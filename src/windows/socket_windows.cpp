#include "common.h"

namespace Netz
{
  namespace SocketPlatform
  {
    void InitPlatform()
    {
      WSADATA wsaData;
      ::WSAStartup(MAKEWORD(2, 0), &wsaData);
    }

    void SetNonBlocking(SocketHandle socket, bool mode)
    {
      
    }

    void ShutdownPlatform()
    {
      ::WSACleanup();
    }

    void Close(SocketHandle socket)
    {
      ::closesocket(socket);
    }

    const char* inet_ntop(int af, const void* src, char* dest, size_t length)
    {
      return ""; // TODO
    }

    int inet_pton(int af, const char* src, void* dest)
    {
      return 0; // TODO
    }
  }
}