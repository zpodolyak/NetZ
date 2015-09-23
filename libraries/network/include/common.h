#ifndef COMMON_H
#define COMMON_H

// std includes
#include <vector>
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
#elif defined(WIN32)
# include "windowsx.h"
# include "ws2tcpip.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

typedef SOCKET SocketHandle;
#else
# error ERROR: Unsupported Platform!
#endif

#include "utils.h"
#include "socket_service.h"
#include "reactor.h"
#include "address.h"
#include "protocol.h"
#include "connection_data.h"
#include "accept_op.h"
#include "connect_op.h"
#include "send_op.h"
#include "receive_op.h"
#include "socket_platform.h"
#include "socket.h"
#include "stream_socket.h"
#include "server_socket.h"

#endif
