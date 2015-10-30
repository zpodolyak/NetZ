#pragma once

namespace NetZ
{
  enum ReactorOps
  {
    read = 0,
    write,
    error,
    max_ops,
    connect = write
  };

  template <typename Callback>
  struct CompletionHandler
  {
    CompletionHandler(Callback _cb)
      : cb(_cb)
    {}

    template <typename... TArgs>
    void operator()(TArgs&&... args)
    {
      cb(std::forward<TArgs>(args)...);
    }
    Callback cb;
  };

  const int REACTOR_QUEUES_SIZE = 3;

  using RunHandler = std::function<void(std::error_code&)>;
 
  class ReactorOperation
  {
    friend class Reactor;
  public:
    ReactorOperation(SocketHandle fd, RunHandler rFunc)
      : descriptor(fd)
      , runFunc(std::move(rFunc))
    {}
    ReactorOperation(const ReactorOperation&) = default;
    virtual ~ReactorOperation() {}

    void RunOperation(std::error_code& ec_)
    {
      runFunc(ec_);
      CompleteOperation();
    }

    virtual void CompleteOperation() {}
  protected:
    SocketHandle descriptor;
    RunHandler runFunc;
    std::error_code ec;
  };
}
#if defined PLATFORM_LINUX && defined HAS_EPOLL
# include "linux/epoll_reactor.h"
#else
# include "windows/select_reactor.h"
#endif

