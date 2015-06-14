#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "common.h"

using Netz::TcpSocket;
using Netz::Tcp;

TEST(SocketTemplateCtor, Open)
{
  TcpSocket s{Tcp()};
  EXPECT_TRUE(s.IsOpen());
}

