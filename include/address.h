#ifndef ADDRESS_H
#define ADDRESS_H

namespace Netz
{
  typedef struct addrinfo addrinfo;

  struct Address
  {
    Address(std::string host_string);

    static Address ResolveHost(const std::string& host_string);

    bool GetAddressInfo(int socket_type, const char* port, addrinfo** res);
    std::string ResolveHost();

    std::string host;
  };
}

#endif
