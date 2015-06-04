#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "common.h"

using Socket = Netz::TcpSocket;

TEST(ContrivedExampleTest, CreateSocket)
{
  auto s = Socket::CreateServerSocket<Netz::ProtocolData<Netz::Protocol::TCP>>(1112);
  EXPECT_NE(INVALID_SOCKET, s);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

#include "../src/socket.cpp"
#include "../src/connection_data.cpp"
#include "../src/address.cpp"
#ifdef PLATFORM_LINUX
# include "../src/linux/socket_linux.cpp"
#endif
