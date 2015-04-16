#ifndef COMMON_H
#define COMMON_H

// std includes
#include <vector>
#include <memory>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <functional>
#include <algorithm>
#include <cassert>

// platform specific includes
#ifdef PLATFORM_LINUX
# include <linux/version.h>
# if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,45)
#   define HAS_EPOLL
#   include <sys/epoll.h>
# endif
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <errno.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <fcntl.h>

# if defined(HAS_EPOLL)
#   include "platform/linux/epoll_event.h"
# endif
#else
# error ERROR: currently only Linux is supported!
#endif


#include "utils.h"
#include "reactor.h"
#include "address.h"
#include "socket.h"

#endif
