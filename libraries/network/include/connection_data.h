#ifndef CONNECTION_DATA_H
#define CONNECTION_DATA_H

namespace NetZ
{
  typedef struct sockaddr_in sockaddr_in;

  struct ConnectionDataV4
  {
    ConnectionDataV4();
    ConnectionDataV4(const AddressV4& addr, unsigned short port);

    unsigned short GetPort() const;
    void SetPort(unsigned short port);
    std::size_t Size() const;

    sockaddr_in data;  
  };

  typedef ConnectionDataV4 ConnectionData;
}

#endif
