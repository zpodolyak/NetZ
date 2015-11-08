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

  Timer::Timer(uint64_t startIn, uint64_t interval, TimerCallback && _handler)
    : startInMs(startIn)
    , periodMs(interval)
    , nextRun(std::chrono::steady_clock::now() + startInMs)
    , handler(std::move(_handler))
    , state(TimerState::Scheduled)
  {
  }

  void Timer::Schedule(uint64_t startIn, uint64_t interval, TimerCallback&& _handler)
  {
    startInMs = Milliseconds(startIn);
    periodMs = Milliseconds(interval);
    nextRun = std::chrono::steady_clock::now() + startInMs;
    handler = std::move(_handler);
    state = TimerState::Scheduled;
  }

  void Timer::Reset()
  {
    startInMs = (IsPeriodic()) ? periodMs : startInMs;
    nextRun = std::chrono::steady_clock::now() + startInMs;
    state = TimerState::Reset;
  }

  Timer::TimerState Timer::Run()
  {
    if (!handler)
    {
      state = TimerState::Error;
      return state;
    }

    auto now = std::chrono::steady_clock::now();
    if (now >= nextRun && state != TimerState::Cancelled)
    {
      state = TimerState::Running;
      handler();
      if (IsPeriodic())
      {
        nextRun = std::chrono::steady_clock::now() + periodMs;
        state = TimerState::Reset;
      }
      else state = TimerState::Finished;
    }
    return state;
  }

  void Timer::Cancel()
  {
    state = TimerState::Cancelled;
  }

  TimerHost::TimerHost()
  {
  }

  Timer* TimerHost::Add(Timer&& timer)
  {
    auto it = queue.emplace(timer.GetNextRun(), std::move(timer));
    return &it->second;
  }

  void TimerHost::Remove(Timer* timer)
  {
    for (auto it = queue.begin(); it != queue.end();)
    {
      if (&it->second == timer)
      {
        it = queue.erase(it);
      }
      else ++it;
    }
  }

  void TimerHost::RunTimers(uint64_t timeout)
  {
    if (queue.empty())
      return;

    Timestamp runTimeLimit = std::chrono::steady_clock::now() + Milliseconds(timeout);
    auto it = queue.begin();

    while (it->first < runTimeLimit)
    {
      if (it == queue.end())
        it = queue.begin();
      if (it->second.Run() != Timer::TimerState::Error)
      {
        auto currentState = it->second.GetState();
        if (currentState == Timer::TimerState::Reset)
          Add(std::move(it->second));
        if (currentState != Timer::TimerState::Scheduled)
        {
          it = queue.erase(it);
          if (queue.empty())
            return;
        }
      }
      else ++it;
    }
  }
}
}
