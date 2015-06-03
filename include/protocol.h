#ifndef PROTOCOL_H
#define PROTOCOL_H

namespace Netz
{
  enum class Protocol
  {
    UDP,
    TCP
  };

  struct ProtocolData
  {
    ProtocolData(int _fam = AF_INET, int _type = SOCK_STREAM, int _prot = IPPROTO_TCP)
     : family(_fam), type(_type), protocol(_prot) 
    {
    }

    ProtocolData(Protocol prot)
    {
      switch(prot)
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
