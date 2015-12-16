#include "timer.h"

namespace
{
  std::mutex lock;
}

namespace NetZ
{
namespace Util
{
  Timer::TimerData::TimerData()
    : startInMs(0)
    , periodMs(0)
    , handler()
  {
  }

  Timer::TimerData::TimerData(uint64_t startIn, uint64_t interval, TimerCallback && _handler)
    : startInMs(startIn)
    , periodMs(interval)
    , handler(std::move(_handler))
  {
  }

  Timer::Timer()
    : timerData()
    , state(TimerState::NotScheduled)
  {
  }

  Timer::Timer(TimerData&& tData)
    : timerData(std::move(tData))
  {
  }

  Timer::Timer(uint64_t startIn, uint64_t interval, TimerCallback && _handler)
    : timerData(startIn, interval, std::move(_handler))
    , nextRun(std::chrono::steady_clock::now() + timerData.startInMs)
    , state(TimerState::Scheduled)
  {
  }

  void Timer::Schedule(TimerData&& tData)
  {
    timerData = std::move(tData);
  }

  void Timer::Schedule(uint64_t startIn, uint64_t interval, TimerCallback&& _handler)
  {
    std::lock_guard<std::mutex> guard(lock);
    timerData.startInMs = Milliseconds(startIn);
    timerData.periodMs = Milliseconds(interval);
    timerData.handler = std::move( _handler );
    nextRun = std::chrono::steady_clock::now() + timerData.startInMs;
    state = TimerState::Scheduled;
  }

  void Timer::Reset()
  {
    std::lock_guard<std::mutex> guard(lock);
    timerData.startInMs = (IsPeriodic()) ? timerData.periodMs : timerData.startInMs;
    nextRun = std::chrono::steady_clock::now() + timerData.startInMs;
    state = TimerState::Reset;
  }

  Timer::TimerState Timer::Run()
  {
    std::unique_lock<std::mutex> guard(lock);
    if (!timerData.handler)
    {
      state = TimerState::Error;
      return state;
    }

    auto now = std::chrono::steady_clock::now();
    if (now >= nextRun && state != TimerState::Cancelled)
    {
      state = TimerState::Running;
      guard.unlock();
      timerData.handler();
      guard.lock();
      if (IsPeriodic() && state != TimerState::Cancelled)
      {
        nextRun = std::chrono::steady_clock::now() + timerData.periodMs;
        state = TimerState::Reset;
      }
      else state = TimerState::Finished;
    }
    return state;
  }

  Timer::TimerState Timer::RunUntil(uint64_t timeout)
  {
    auto runTimeLimit = std::chrono::steady_clock::now() + Milliseconds(timeout);

    while (nextRun < runTimeLimit)
    {
      Run();
      if (state == Timer::TimerState::Error || state == Timer::TimerState::Finished)
        return state;
    }
    return state;
  }

  void Timer::RunInThread()
  {
    std::thread([this]()
    {
      while (state != TimerState::Cancelled && state != TimerState::Finished)
      {
        Run();
      }
    }).detach();
  }

  void Timer::Cancel()
  {
    std::lock_guard<std::mutex> guard(lock);
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
        else
        {
          it->second.Cancel();
          ++it;
        }
      }
      else ++it;
    }
  }

  void TimerHost::Reset(TimerID timerID)
  {
    for (auto it = queue.begin(); it != queue.end();)
    {
      if (it->second.GetID() == timerID 
          && it->second.GetState() != Timer::TimerState::Running)
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
