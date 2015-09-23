#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "libraries/network/include/common.h"

TEST(ContrivedExampleTest, CreateSocket)
{
  auto s = NetZ::CreateServerSocket<NetZ::ProtocolData<NetZ::Protocol::TCP>>(1112);
  EXPECT_NE(INVALID_SOCKET, s);
}

int main(int argc, char** argv)
{
  NetZ::SocketPlatform::InitPlatform();
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

#include "libraries/network/src/socket.cpp"
#include "libraries/network/src/connection_data.cpp"
#include "libraries/network/src/address.cpp"
#ifdef PLATFORM_LINUX
# include "libraries/network/src/linux/socket_linux.cpp"
#else
# include "../src/windows/socket_windows.cpp"
#endif
