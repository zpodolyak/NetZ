#ifndef REACTOR_H
#define REACTOR_H

namespace Netz
{
  enum class ReactorOps
  {
    read = 0,
    write,
    error,
    max_ops
  };

  const int REACTOR_QUEUE_SIZE = 3;

  struct ReactorOperation
  {

  };
}

#if defined PLATFORM_LINUX && defined HAS_EPOLL
#  include "linux/epoll_reactor.h"
#else
#  include "select_reactor.h"
#endif

#endif

