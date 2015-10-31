#pragma once

// std includes
#include <vector>
#include <set>
#include <deque>
#include <memory>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cinttypes>
#include <sstream>
#include <functional>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <system_error>

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

// common types
typedef int SocketHandle;
# define INVALID_SOCKET -1
# define SOCKET_ERROR -1

template <typename T, typename... Types>
inline std::unique_ptr<T> make_unique(Types&&... args)
{
  return std::unique_ptr<T>(new T(std::forward<Types>(args)...));
}

#elif defined(WIN32)
# include "windowsx.h"
# include "ws2tcpip.h"

using std::make_unique;

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

typedef SOCKET SocketHandle;
#else
# error ERROR: Unsupported Platform!
#endif

#include "utils.h"

