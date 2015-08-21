#pragma once

namespace Netz
{
  enum ReactorOps
  {
    read = 0,
    write,
    error,
    max_ops,
    connect = write
  };

  const int REACTOR_QUEUES_SIZE = 3;

  using RunHandler = std::function<void(std::error_code&)>;
  using CompletionHandler = void(*)(const std::error_code&);

  class ReactorOperation
  {
    friend class Reactor;
  public:
    ReactorOperation(SocketHandle fd, RunHandler rFunc, CompletionHandler cFunc)
      : descriptor(fd)
      , run(std::move(rFunc))
      , complete(cFunc)
    {}

    void RunOperation(std::error_code& ec_)
    {
      run(ec_);
    }

    void CompleteOperation(const std::error_code& ec_)
    {
      complete(ec_);
    }
  private:
    SocketHandle descriptor;
    RunHandler run;
    CompletionHandler complete;
    std::error_code ec;
  };
}

#if defined PLATFORM_LINUX && defined HAS_EPOLL
#  include "linux/epoll_reactor.h"
#else
#  include "select_reactor.h"
#endif

