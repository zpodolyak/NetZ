#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "libraries/network/include/timer.h"
#include <mutex>

using NetZ::Util::Timer;
using NetZ::Util::TimerHost;
using NetZ::Util::Milliseconds;

class TimerHostTest : public TimerHost
{
public:
  using TimerHost::queue;
};

struct MockCalledBackObject
{
  MOCK_METHOD0(CallMe, void());
};

struct TestTimerHost : public ::testing::Test
{
  TestTimerHost()
    : host()
  {
  }

  MockCalledBackObject cObj;
  TimerHostTest host;
  int maxCall = 3, callCounter = 0;
};

TEST_F(TestTimerHost, OneShotInTwoSecs)
{
  EXPECT_CALL(cObj, CallMe()).Times(1);
  host.Add(Timer(2000, 0, [this]() { cObj.CallMe(); }));
  EXPECT_EQ(host.queue.size(), 1);
  while (host.HasTimers())
    host.RunTimers();
  EXPECT_EQ(host.queue.size(), 0);
}

TEST_F(TestTimerHost, PeriodicEveryTwoSecs)
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

TEST_F(TestTimerHost, PeriodicCancelAfterOne)
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

TEST_F(TestTimerHost, PeriodicReset)
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

TEST(SingleTimer, TestSingleTimer)
{
  MockCalledBackObject cObj;
  EXPECT_CALL(cObj, CallMe()).Times(1);
  Timer t(2000, 0, [&cObj]() { cObj.CallMe(); });
  while (t.GetState() != Timer::TimerState::Finished)
    t.RunUntil();
  EXPECT_CALL(cObj, CallMe()).Times(3);
  auto periodCount = 0;
  t.Schedule(2000, 1000, [&cObj, &periodCount]() { cObj.CallMe(); ++periodCount; });
  while (periodCount < 3)
    t.RunUntil();
  EXPECT_CALL(cObj, CallMe() ).Times(2);
  t.Reset();
  auto now = std::chrono::steady_clock::now();
  auto cancelAt = now + Milliseconds(2100);
  while (t.GetState() != Timer::TimerState::Cancelled)
  {
    t.RunUntil();
    now = std::chrono::steady_clock::now();
    if (cancelAt < now)
      t.Cancel();
  }
  EXPECT_CALL(cObj, CallMe()).Times(3);
  t.Reset();
  now = std::chrono::steady_clock::now();
  auto resetAt = now + Milliseconds(2100);
  while (periodCount < 8)
  {
    t.RunUntil();
    now = std::chrono::steady_clock::now();
    if (resetAt < now)
    {
      t.Reset();
      resetAt = now + Milliseconds(2100);
    }
  }
}

TEST(SingleTimerThread, TestTimerThread)
{
  MockCalledBackObject cObj;
  EXPECT_CALL(cObj, CallMe()).Times(1);
  Timer t(Timer::TimerData(2000, 0, [&cObj]() { cObj.CallMe(); }));
  t.RunInThread();
  while (t.GetState() != Timer::TimerState::Finished);
  EXPECT_CALL(cObj, CallMe()).Times(3);
  auto periodCount = 0;
  t.Schedule(2000, 1000, [&cObj, &periodCount]() 
  { 
    cObj.CallMe(); 
    ++periodCount; 
  });
  t.RunInThread();
  while (periodCount < 3);
  t.Cancel();
}

#include "libraries/network/src/timer.cpp"
