#pragma once

#include "protocol.h"

namespace NetZ
{
  template <typename ProtocolType>
  class DatagramSocket : public SocketBase
  {
    typedef ProtocolType protocol_type;

  public:
    DatagramSocket()
      : SocketBase()
    {
    }

    DatagramSocket(SocketService* _service)
      : SocketBase(_service)
    {
    }

    DatagramSocket(const protocol_type& prot)
      : SocketBase(prot)
    {
    }

    DatagramSocket(SocketService* _service, const protocol_type& prot)
      : SocketBase(_service, prot)
    {
    }

    int SendTo(const char* buffer, int length, int msg_flags, const ConnectionData& connData)
    {
      ClearLastError();
      std::error_code ec;
      int bytes = ErrorWrapper(::sendto(socket, buffer, length, msg_flags, (const sockaddr*)&connData.data, sizeof(sockaddr_in)), ec);
#ifndef WIN32
      if (ec == std::error_code((int)std::errc::resource_unavailable_try_again, std::system_category())
        || ec == std::error_code((int)std::errc::operation_would_block, std::system_category()))
#else
      if (ec == std::errc::resource_unavailable_try_again
        || ec == std::errc::operation_would_block)
#endif
        return SOCKET_ERROR;
      return bytes;
    }

    template <typename Handler>
    void SendTo(const char* buffer, int length, int msg_flags, const ConnectionData& connData, Handler&& handler)
    {
      if (socket == INVALID_SOCKET || !service)
        return;
      service->RegisterOperation(ReactorOps::write, new SendToOperation<Handler, ConnectionData>(buffer, length, msg_flags, socket, connData, std::forward<Handler>(handler)));
    }

    int ReceiveFrom(char* buffer, int length, int msg_flags, ConnectionData& connData)
    {
      ClearLastError();
      std::error_code ec;
      auto connLen = int(connData.Size());
      int bytes = ErrorWrapper(::recvfrom(socket, buffer, length, msg_flags, (sockaddr*)&connData.data, &connLen), ec);
#ifndef WIN32
      if (ec == std::error_code((int)std::errc::resource_unavailable_try_again, std::system_category())
        || ec == std::error_code((int)std::errc::operation_would_block, std::system_category()))
#else
      if (ec == std::errc::resource_unavailable_try_again
        || ec == std::errc::operation_would_block)
#endif
        return SOCKET_ERROR;
      return bytes;
    }


    template <typename Handler>
    void ReceiveFrom(char* buffer, int length, int msg_flags, ConnectionData& connData, Handler&& handler)
    {
      if (socket == INVALID_SOCKET || !service)
        return;
      service->RegisterOperation(ReactorOps::read, new ReceiveFromOperation<Handler, ConnectionData>(buffer, length, msg_flags, socket, connData, std::forward<Handler>(handler)));
    }
  };

  using UdpSocket = DatagramSocket<Udp>;

}
