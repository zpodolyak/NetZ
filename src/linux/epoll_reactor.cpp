#include "common.h"

#ifdef HAS_EPOLL
#include "linux/epoll_reactor.h"

namespace Netz
{

namespace
{
  std::vector<ReactorOperation*> cleanupOpsOnExit;
}

Reactor::Reactor()
{
  epoll_fd = epoll_create1(0);
  if (epoll_fd == INVALID_SOCKET)
    PrintError("epoll_create failed");
}

Reactor::~Reactor()
{
  Stop();
  if (epoll_fd != INVALID_SOCKET)
    close(epoll_fd);
  for (auto it : cleanupOpsOnExit)
    delete it;
}

void Reactor::RegisterDescriptorOperation(int type, ReactorOperation* op)
{
  if (!(type < ReactorOps::max_ops) && op->descriptor == INVALID_SOCKET)
    return;

  epoll_event ev = { 0,{ 0 } };
  ev.data.ptr = op;
  auto it = taskQueue[type].emplace(std::piecewise_construct, std::forward_as_tuple(op->descriptor), std::forward_as_tuple());
  if (it.second)
  {
    ev.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLPRI | EPOLLET;
    int result = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, op->descriptor, &ev);
    if (result != 0)
      PrintError("Failed to add descriptor to epoll.");
  }
  if (it.first->second.empty())
  {
    if (type == ReactorOps::write)
      ev.events |= EPOLLOUT;
    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, op->descriptor, &ev);
  }
  it.first->second.push_back(op);
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

void Reactor::Run(int timeout)
{
  std::error_code ec;
  epoll_event events[128];
  std::vector<ReactorOperation*> readyOps;

  int num_events = epoll_wait(epoll_fd, events, 128, timeout);

  for (int i = 0; i < num_events; ++i)
  {
    if (events[i].events & (EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLERR | EPOLLHUP))
    {
      void* ptr = events[i].data.ptr;
      auto op = static_cast<ReactorOperation*>(ptr);
      readyOps.push_back(op);
    }
  }

  for (int i = ReactorOps::max_ops - 1; i >= 0; --i)
    for (std::size_t j = 0; j < readyOps.size(); ++j)
      for (auto it = taskQueue[i][readyOps[j]->descriptor].begin(); it != taskQueue[i][readyOps[j]->descriptor].end();)
      {
        auto rOp = *it;
        rOp->RunOperation(ec);
        cleanupOpsOnExit.push_back(rOp);
        it = taskQueue[i][readyOps[j]->descriptor].erase(it);
      }
} 

}
#endif

