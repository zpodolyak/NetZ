#include "timer.h"

namespace NetZ
{
namespace Util
{
  SimpleTimer::SimpleTimer()
    : running(false)
  {
  }

  SimpleTimer::~SimpleTimer()
  {
    running = false;
  }
  
  void SimpleTimer::Wait(uint64_t starting, Callback&& handler)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(starting));
    if (handler)
      handler();
  }

  void SimpleTimer::AsyncWait(uint64_t starting, Callback&& handler, uint64_t interval)
  {
    running = true;
    if (!handler)
      return;

    std::thread([this, starting, handler, &interval]()
    {
      auto next = std::chrono::steady_clock::now() + Milliseconds(starting);
      std::condition_variable cond;
      if (interval == 0)
      {
        std::this_thread::sleep_for(Milliseconds(starting));
        handler();
      }
      else while (running)
      {
        auto now = std::chrono::steady_clock::now();
        if (now >= next)
        {
          handler();
          next = now + Milliseconds(interval);
        }
        else cond.wait_until(std::unique_lock<std::mutex>(), next);
      }
    }).detach();
  }

  void SimpleTimer::Cancel()
  {
    running = false;
  }
}
}
