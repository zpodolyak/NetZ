#pragma once

#include <chrono>
#include <functional>
#include <map>

namespace NetZ
{
namespace Util
{
  using Milliseconds = std::chrono::milliseconds;
  using Timestamp = std::chrono::time_point<std::chrono::steady_clock>;
  using TimerCallback = std::function<void()>;

  class Timer
  {
  public:
    Timer();
    Timer(uint64_t startIn, uint64_t interval, TimerCallback&& _handler);
    Timer(Timer&&) = default;
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

    enum class TimerState
    {
      NotScheduled,
      Scheduled,
      Running,
      Finished,
      Cancelled,
      Reset,
      Error
    };

    void Schedule(uint64_t startIn, uint64_t interval, TimerCallback&& _handler);
    void Reset();
    TimerState Run();
    void Cancel();
    bool IsPeriodic() const { return periodMs > Milliseconds(0); }
    const Timestamp& GetNextRun() const { return nextRun; }
    const TimerState GetState() const { return state; }
  private:
    Milliseconds startInMs, periodMs;
    Timestamp nextRun;
    TimerCallback handler;
    TimerState state;
  };

  class TimerHost
  {
  public:
    TimerHost();

    Timer* Add(Timer&& timer);
    void Remove(Timer* timer);
    void RunTimers(uint64_t timeout = 200);
  private:
    std::multimap<Timestamp, Timer> queue;
  };
}
}