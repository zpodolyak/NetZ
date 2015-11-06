#pragma once

#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace NetZ
{
namespace Util
{
  class SimpleTimer
  {
    typedef std::chrono::time_point<std::chrono::steady_clock> Timestamp;
    typedef std::chrono::milliseconds Milliseconds;
    typedef std::function<void()> Callback;
  public:
    SimpleTimer();
    ~SimpleTimer();
    SimpleTimer(const SimpleTimer&) = delete;
    SimpleTimer& operator=(const SimpleTimer&) = delete;

    void Wait(uint64_t starting, Callback&& _handler);
    void AsyncWait(uint64_t starting, Callback&& _handler, uint64_t period = 0);
    void Cancel();
  private:
    bool running;
  };
}
}