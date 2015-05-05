#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "common.h"

using Socket = Netz::Socket;

TEST(ContrivedExampleTest, CreateSocket)
{
  auto s = Socket::CreateServerSocket(SOCK_STREAM, 1112);
  EXPECT_NE(0, s->socket);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

#include "../src/socket.cpp"
#include "../src/address.cpp"
