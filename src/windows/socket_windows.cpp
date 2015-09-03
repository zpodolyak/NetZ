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
      u_long iMode = mode ? 1 : 0;
      ::ioctlsocket(socket, FIONBIO, &iMode);
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
      if (af == AF_INET)
      {
        if (!dest)
          return nullptr;

        const uint8_t* bytes = static_cast<const uint8_t*>(src);
        std::snprintf(dest, length, "%u.%u.%u.%u", bytes[0], bytes[1], bytes[2], bytes[3]);
        return dest;
      }
      return nullptr;
    }

    int inet_pton(int af, const char* src, void* dest)
    {
      uint8_t* bytes = (uint8_t*)dest;
      if (af == AF_INET)
      {
        uint8_t b1, b2, b3, b4;
        if (std::sscanf(src, "%hhu.%hhu.%hhu.%hhu", &b1, &b2, &b3, &b4) != 4 ||
           (b1 > 255 || b2 > 255 || b3 > 255 || b4 > 255))
        {
          return 0;
        }
        bytes[0] = b1;
        bytes[1] = b2;
        bytes[2] = b3;
        bytes[3] = b4;
      }
      else
      {
        ::WSASetLastError(WSAEAFNOSUPPORT);
        return -1;
      }
      return 1;
    }
  }
}

