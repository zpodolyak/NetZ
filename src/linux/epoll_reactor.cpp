#include "common.h"

#ifdef HAS_EPOLL
#include "linux/epoll_reactor.h"

namespace Netz
{

Reactor::Reactor()
{
  epoll_fd = epoll_create(20000);
  if (epoll_fd != INVALID_SOCKET)
    ::fcntl(epoll_fd, F_SETFD, FD_CLOEXEC);
}

Reactor::~Reactor()
{
  Stop();
  if (epoll_fd != INVALID_SOCKET)
    close(epoll_fd);
}

void Reactor::RegisterDescriptor(int type, ReactorOperation* op)
{
  if (!(type < ReactorOps::max_ops) && op->descriptor == INVALID_SOCKET)
    return;

  auto it = taskQueue[type].emplace(std::piecewise_construct, std::forward_as_tuple(op->descriptor), std::forward_as_tuple());
  it.first->second.push_back(op);

  epoll_event ev = { 0,{ 0 } };
  ev.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLPRI | EPOLLET;
  ev.data.ptr = op;
  int result = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, op->descriptor, &ev);
  if (result != 0)
    PrintError("Failed to add descriptor to epoll.");
}

void Reactor::CancelDescriptor(SocketHandle fd)
{
  for (int i = 0; i < REACTOR_QUEUES_SIZE; ++i)
  {
    auto it = taskQueue[i].find(fd);
    if (it != std::end(taskQueue[i]))
    {
      while (!it->second.empty())
      {
        auto op = it->second.front();
        op->ec = std::make_error_code(std::errc::operation_canceled);

        epoll_event ev = { 0,{ 0 } };
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, op->descriptor, &ev);

        delete it->second.front();
        it->second.pop_front();
      }
      taskQueue[i].erase(it);
    }
  }
}

}
#endif

