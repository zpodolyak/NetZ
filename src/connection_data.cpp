#include "common.h"

namespace Netz
{

  ConnectionDataV4::ConnectionDataV4()
  {
    data.sin_family = AF_INET;
    data.sin_port = 0;
    data.sin_addr.s_addr = INADDR_ANY;
  }

  ConnectionDataV4::ConnectionDataV4(const AddressV4& addr, unsigned short port)
  {
    data.sin_family = AF_INET;
    data.sin_port = ::htons(port); 
    data.sin_addr.s_addr = addr.address.s_addr;
  }

  unsigned short ConnectionDataV4::GetPort() const
  {
    return ::ntohs(data.sin_port);
  }

  void ConnectionDataV4::SetPort(unsigned short port)
  {
    data.sin_port = ::htons(port);
  }
  
  std::size_t ConnectionDataV4::Size() const
  {
    return sizeof(data);
  }
}


