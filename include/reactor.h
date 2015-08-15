#ifndef REACTOR_H
#define REACTOR_H

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

  class ReactorOperation
  {
    using CompletionHandler = void(*)(const std::error_code&);
    friend class Reactor;
  public:
    ReactorOperation(SocketHandle fd, CompletionHandler cFunc)
      : descriptor(fd)
      , comp(cFunc)
    {}

    void RunOperation(const std::error_code& ec_)
    {
      comp(ec_);
    }
  private:
    SocketHandle descriptor;
    CompletionHandler comp;
    std::error_code ec;
  };
}

#if defined PLATFORM_LINUX && defined HAS_EPOLL
#  include "linux/epoll_reactor.h"
#else
#  include "select_reactor.h"
#endif

#endif

