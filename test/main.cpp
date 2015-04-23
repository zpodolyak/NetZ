#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "common.h"

using Socket = Netz::Socket;

TEST(ContrivedExampleTest, CreateSocket)
{
  Reactor r;
  Socket s(r, SOCK_STREAM, 1112);
  int fd = s.CreateServerSocket();
  EXPECT_NE(0, fd);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
