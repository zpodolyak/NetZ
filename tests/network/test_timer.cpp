#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "libraries/network/include/timer.h"

using NetZ::Util::Timer;
using NetZ::Util::TimerHost;
using NetZ::Util::Milliseconds;

class TestTimerHost : public TimerHost
{
public:
  using TimerHost::queue;
};

struct MockCalledBackObject
{
  MOCK_METHOD0(CallMe, void());
};

struct TestTimer : public ::testing::Test
{
  TestTimer()
    : host()
  {
  }

  MockCalledBackObject cObj;
  TestTimerHost host;
  int maxCall = 3, callCounter = 0;
};

TEST_F(TestTimer, OneShotInTwoSecs)
{
  EXPECT_CALL(cObj, CallMe()).Times(1);
  host.Add(Timer(2000, 0, [this]() { cObj.CallMe(); }));
  EXPECT_EQ(host.queue.size(), 1);
  while (host.HasTimers())
    host.RunTimers();
  EXPECT_EQ(host.queue.size(), 0);
}

TEST_F(TestTimer, PeriodicEveryTwoSecs)
{
  EXPECT_CALL(cObj, CallMe()).Times(3);
  host.Add(Timer(2000, 2000, [this]() 
  { 
    cObj.CallMe(); 
    --maxCall;
    ++callCounter;
  }));
  while (host.HasTimers() && maxCall > 0)
    host.RunTimers();
  EXPECT_EQ(maxCall, 0);
  EXPECT_EQ(callCounter, 3);
}

TEST_F(TestTimer, PeriodicCancelAfterOne)
{
  EXPECT_CALL(cObj, CallMe()).Times(1);
  auto timer = host.Add(Timer(2000, 2000, [this]()
  {
    cObj.CallMe();
  }));
  auto now = std::chrono::steady_clock::now();
  auto cancelAt = now + Milliseconds(2100);
  while (host.HasTimers())
  {
    host.RunTimers();
    now = std::chrono::steady_clock::now();
    if (cancelAt < now)
      host.Cancel(timer);
  }
  EXPECT_FALSE(host.HasTimers());
}

TEST_F(TestTimer, PeriodicReset)
{
  EXPECT_CALL(cObj, CallMe()).Times(1);
  auto timer = host.Add(Timer(2000, 2000, [this]()
  {
    cObj.CallMe();
  }));
  auto now = std::chrono::steady_clock::now();
  auto nextReset = now + Milliseconds(1000);
  auto resetCount = 0;
  while (host.HasTimers())
  {
    host.RunTimers();
    now = std::chrono::steady_clock::now();
    if (nextReset < now)
    {
      host.Reset(timer);
      nextReset = now + Milliseconds(2000);
      ++resetCount;
    }
    if (resetCount > 1)
      host.Cancel(timer);
  }
  EXPECT_FALSE(host.HasTimers());
}

#include "libraries/network/src/timer.cpp"
