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
  using CompletionHandler = std::function<void(const std::error_code&)>;
  using DataCompletionHandler = std::function<void(int, const std::error_code&)>;
  template <typename SocketType>
  using AcceptCompletionHandler = std::function<void(SocketType&, const std::error_code&)>;

  class ReactorOperation
  {
    friend class Reactor;
  public:
    ReactorOperation(SocketHandle fd, RunHandler rFunc)
      : descriptor(fd)
      , run(std::move(rFunc))
    {}
    ReactorOperation(const ReactorOperation&) = default;
    virtual ~ReactorOperation() {}

    void RunOperation(std::error_code& ec_)
    {
      run(ec_);
      CompleteOperation();
    }

    virtual void CompleteOperation() {}
  protected:
    SocketHandle descriptor;
    RunHandler run;
    std::error_code ec;
  };
}
#if defined PLATFORM_LINUX && defined HAS_EPOLL
# include "linux/epoll_reactor.h"
#else
# include "select_reactor.h"
#endif

