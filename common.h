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

// Platform includes
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

// Program includes
#include "common/utils.h"
#include "common/epoll_event.h"
#include "common/reactor.h"
#include "include/socket_address.h"
#include "include/s_socket.h"
#include "include/iconnection_handler.h"
#include "include/socket_server.h"
#include "include/game_connection.h"

#endif
