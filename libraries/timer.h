#pragma once

#include <chrono>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace NetZ
{
namespace Util
{
  class Timer
  {
    typedef std::chrono::time_point<std::chrono::steady_clock> Timestamp;
    typedef std::chrono::milliseconds Milliseconds;
    typedef std::function<void()> Callback;

    enum class TimerState
    {
      NotScheduled,
      Scheduled,
      Running,
      Finished,
      Cancelled,
      Error
    };

  public:
    Timer();
    Timer(uint64_t startIn, uint64_t interval, Callback&& _handler);
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

    void Schedule(uint64_t startIn, uint64_t interval, Callback&& _handler);
    TimerState Run();
    void Cancel();
  private:
    Milliseconds startInMs, periodMs;
    Timestamp nextRun;
    Callback handler;
    TimerState state;
  };

  class TimerHost
  {

  };
}
}