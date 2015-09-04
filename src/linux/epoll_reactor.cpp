#include "common.h"

#ifdef HAS_EPOLL
#include "linux/epoll_reactor.h"

namespace Netz
{

Reactor::Reactor()
{

}

Reactor::~Reactor()
{
  Stop();
}

}
#endif

