#pragma once

namespace NetZ
{
  typedef struct addrinfo addrinfo;
  typedef struct in_addr ipv4_addr;

  struct AddressV4
  {
    AddressV4()
    {
      address.s_addr = 0;
    }

    static AddressV4 FromString(const char* ipaddress);
    static bool ResolveFromHostname(const char* hostname, int socket_type, const char* port, addrinfo** res);

    std::string ToString() const;

    ipv4_addr address;
  };

  typedef AddressV4 Address;
}

