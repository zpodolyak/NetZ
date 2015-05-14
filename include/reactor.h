#ifndef REACTOR_H
#define REACTOR_H

#if defined PLATFORM_LINUX && defined HAS_EPOLL
#  include "linux/epoll_reactor.h"
#endif

#endif

