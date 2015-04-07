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

// platform specific includes
#ifdef PLATFORM_LINUX
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <sys/epoll.h>
  #include <unistd.h>
  #include <errno.h>
  #include <netdb.h>
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <fcntl.h>
  
  #include "platform/linux/epoll_event.h"
#endif


#include "utils.h"
#include "reactor.h"
#include "socket_address.h"
#include "s_socket.h"
#include "iconnection_handler.h"
#include "socket_server.h"
#include "game_connection.h"

#endif
