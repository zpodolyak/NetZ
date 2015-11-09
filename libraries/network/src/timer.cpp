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
      if (IsPeriodic() && state != TimerState::Cancelled)
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

  TimerID TimerHost::Add(Timer&& timer)
  {
    auto it = queue.emplace(timer.GetNextRun(), std::move(timer));
    auto& tmr = it->second;
    if (tmr.GetID() == -1) 
      tmr.SetID(timerCounter++);
    return it->second.GetID();
  }

  void TimerHost::Cancel(TimerID timerID)
  {
    for (auto it = queue.begin(); it != queue.end();)
    {
      if (it->second.GetID() == timerID)
      {
        if (it->second.GetState() != Timer::TimerState::Running)
          it = queue.erase(it);
        else it->second.Cancel();
      }
      else ++it;
    }
  }

  void TimerHost::Reset(TimerID timerID)
  {
    for (auto it = queue.begin(); it != queue.end();)
    {
      if (it->second.GetID() == timerID)
      {
        it->second.Reset();
        Add(std::move(it->second));
        it = queue.erase(it);
        return;
      }
      else ++it;
    }
  }

  void TimerHost::RunTimers(uint64_t timeout)
  {
    if (!HasTimers())
      return;

    auto runTimeLimit = std::chrono::steady_clock::now() + Milliseconds(timeout);
    auto it = queue.begin();

    while (it->first < runTimeLimit)
    {
      if (it == queue.end())
        it = queue.begin();
      auto currentState = it->second.Run();
      if (currentState != Timer::TimerState::Error)
      {
        if (currentState == Timer::TimerState::Reset)
          Add(std::move(it->second));
        if (currentState != Timer::TimerState::Scheduled)
        {
          it = queue.erase(it);
          if (!HasTimers())
            return;
        }
      }
      else ++it;
    }
  }
}
}
