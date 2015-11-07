#include "timer.h"

namespace NetZ
{
namespace Util
{
  Timer::Timer()
    : startInMs(0)
    , periodMs(0)
    , handler()
    , state(TimerState::NotScheduled)
  {
  }

  Timer::Timer(uint64_t startIn, uint64_t interval, Callback && _handler)
    : startInMs(startIn)
    , periodMs(interval)
    , nextRun(std::chrono::steady_clock::now() + startInMs)
    , handler(std::move(_handler))
    , state(TimerState::Scheduled)
  {
  }

  void Timer::Schedule(uint64_t startIn, uint64_t interval, Callback&& _handler)
  {
    startInMs = Milliseconds(startIn);
    periodMs = Milliseconds(interval);
    nextRun = std::chrono::steady_clock::now() + startInMs;
    handler = std::move(_handler);
    state = TimerState::Scheduled;
  }

  Timer::TimerState Timer::Run()
  {
    if (!handler)
    {
      state = TimerState::Error;
      return state;
    }

    auto now = std::chrono::steady_clock::now();
    if (now >= nextRun)
    {
      state = TimerState::Running;
      handler();
      if (periodMs > Milliseconds(0) && state != TimerState::Cancelled)
      {
        nextRun = std::chrono::steady_clock::now() + periodMs;
        state = TimerState::Scheduled;
      }
      else state = TimerState::Finished;
    }
    return state;
  }

  void Timer::Cancel()
  {
    state = TimerState::Cancelled;
  }
}
}
