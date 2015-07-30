#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "common.h"

using Netz::TcpSocket;
using Netz::Tcp;
using Netz::Address;
using Netz::ConnectionData;
using Netz::SocketOption;

class Test_TcpSocket : public ::testing::Test
{
public:
  Test_TcpSocket() 
   : sock(Tcp())
   , conn(Address::FromString("127.0.0.1"), 1112)
   {
   }
  
  TcpSocket sock;
  ConnectionData conn;
};

TEST_F(Test_TcpSocket, Open)
{
  EXPECT_NE(sock.IsOpen(), false);
  EXPECT_EQ(conn.GetPort(), 1112);
}

TEST_F(Test_TcpSocket, GetSetOption)
{
  SocketOption reuse(SOL_SOCKET, SO_REUSEADDR, 1);
  SocketOption get_reuse;
  get_reuse.level = SOL_SOCKET;
  get_reuse.name = SO_REUSEADDR;
  EXPECT_EQ(0, sock.SetSocketOption(reuse).value());
  EXPECT_EQ(0, sock.GetSocketOption(get_reuse).value());
  EXPECT_TRUE(reuse == get_reuse);
}

