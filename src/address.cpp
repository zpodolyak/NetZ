#include "common.h"

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
    return AddressV4(); // TODO
  }

  std::string AddressV4::ToString() const
  {
    return std::string(); // TODO
  }
}
