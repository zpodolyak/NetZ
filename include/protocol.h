#ifndef PROTOCOL_H
#define PROTOCOL_H

namespace Netz
{
  struct ProtocolData
  {
    ProtocolData(int _fam = AF_INET, int _type = SOCK_STREAM, int _prot = IPPROTO_TCP)
     : family(_fam), type(_type), protocol(_prot) 
    {
    }

    int family;
    int type;
    int protocol;
  };
}

#endif
