#include "common.h"

namespace Netz
{
  Address::Address(std::string host_string)
  : host(std::move(host_string))
  {
  }

  bool Address::GetAddressInfo(int socket_type, const char* port, addrinfo** res)
  {
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = socket_type;
    
    if(host.empty())
      hints.ai_flags = AI_PASSIVE;

    if ((getaddrinfo((!host.empty()) ? host.c_str() : nullptr, port, &hints, res)) != 0) 	
    {
      PrintError("getaddrinfo");
      return false;
    }
    return true;
  }

  Address Address::ResolveHost(const std::string & host_string)
  {
    Address addr{ host_string };
    addr.ResolveHost();
    return addr;
  }

  std::string Address::ResolveHost()
  {
    std::string ipstr;
    addrinfo *res = nullptr, *p = nullptr;

    if (GetAddressInfo(SOCK_STREAM, nullptr, &res))
    {
      struct in_addr addr;
      for(p=res; p!=nullptr; p=p->ai_next)
      {
        if(p->ai_family!=AF_INET)
          continue;
        addr =((const struct sockaddr_in*)p->ai_addr)->sin_addr;
        break;
      }
    
      char str[128];
      inet_ntop(p->ai_family, &addr, str, sizeof(ipstr));
      ipstr = str;
    }
    freeaddrinfo(res);
    return ipstr;
  }
}
