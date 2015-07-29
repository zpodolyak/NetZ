#include "common.h"

#ifdef WIN32
# pragma comment(lib, "ws2_32.lib")
#endif

namespace Netz
{
  bool AddressV4::ResolveFromHostname(const char* hostname, int socket_type, const char* port, addrinfo** res)
  {
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = socket_type;
    
    if(!hostname)
      hints.ai_flags = AI_PASSIVE;

    if ((::getaddrinfo((hostname) ? hostname : nullptr, port, &hints, res)) != 0) 	
    {
      PrintError("getaddrinfo");
      return false;
    }
    return true;
  }

  AddressV4 AddressV4::FromString(const char* hostname)
  {
    AddressV4 tmp;
#ifndef WIN32 // TODO
    if (SocketPlatform::inet_pton(AF_INET, hostname, &tmp.address) < 0)
      return AddressV4();
#endif
    return tmp;
  }

  std::string AddressV4::ToString() const
  {
    char addr_str[128];
#ifndef WIN32 // TODO
    const char* addr = SocketPlatform::inet_ntop(AF_INET, &address, addr_str, sizeof(addr_str));
#else
    const char* addr = nullptr;
#endif
    if (!addr)
      return std::string();
    return addr;
  }
}
