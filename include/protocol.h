#ifndef PROTOCOL_H
#define PROTOCOL_H

namespace Netz
{
  enum class Protocol
  {
    UDP,
    TCP
  };
  
  template <Protocol pr>
  class ProtocolData
  {
  public:
    ProtocolData()
    {
      switch(pr)
      {
        case Protocol::TCP:
        {
          family = AF_INET;
          type = SOCK_STREAM;
          protocol = IPPROTO_TCP;
          break;
        }
        case Protocol::UDP:
        {
          family = AF_INET;
          type = SOCK_DGRAM;
          protocol = IPPROTO_UDP;
          break;
        }
        default:
          PrintError("Unsupported protocol");
      }
    }

    int family;
    int type;
    int protocol;
  };

}

#endif
