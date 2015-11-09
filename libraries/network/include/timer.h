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
  using TimerID = int;

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
    bool IsPeriodic() const { return periodMs > Milliseconds(0); }
    const Timestamp& GetNextRun() const { return nextRun; }
    TimerState GetState() const { return state; }
    void SetID(TimerID id) { timerID = id; }
    int GetID() const { return timerID; }
    TimerState Run();
    void Cancel();
    void Reset();
  private:
    Milliseconds startInMs, periodMs;
    Timestamp nextRun;
    TimerCallback handler;
    TimerState state;
    TimerID timerID = -1;
  };

  class TimerHost
  {
  public:
    TimerHost();
    TimerHost(const TimerHost&) = delete;
    TimerHost& operator=(const TimerHost&) = delete;

    TimerID Add(Timer&& timer);
    void Cancel(TimerID timerID);
    void Reset(TimerID timerID);
    void RunTimers(uint64_t timeout = 200);
    bool HasTimers() const { return !queue.empty(); }
  protected:
    typedef std::multimap<Timestamp, Timer> TimerQueue;
    TimerQueue queue;
    int timerCounter = 0;
  };
}
}