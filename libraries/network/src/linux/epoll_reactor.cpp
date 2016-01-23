#include "libraries/common.h"
#include "libraries/network/include/network.h"

#ifdef HAS_EPOLL
#include "linux/epoll_reactor.h"

namespace
{
  struct ReactorOpsCleanup
  {
    std::vector<NetZ::ReactorOperation*> cl;

    ~ReactorOpsCleanup()
    {
      for (auto op : cl)
        delete op;
      cl.clear();
    }
  };
}

namespace NetZ
{

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
}

void Reactor::RegisterOperation(int type, ReactorOperation* op)
{
  if (!(type < ReactorOps::max_ops) && op->descriptor == INVALID_SOCKET)
    return;

  auto it = taskQueue[type].emplace(std::piecewise_construct, std::forward_as_tuple(op->descriptor), std::forward_as_tuple());
  if (it.first->second.empty())
  {
    epoll_event ev = { 0,{ 0 } };
    ev.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLPRI | EPOLLET;
    ev.data.ptr = op;
    if (type == ReactorOps::write)
      ev.events |= EPOLLOUT;
    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, op->descriptor, &ev);
  }
  it.first->second.push_back(op);
}

void Reactor::RegisterDescriptor(SocketHandle fd)
{
  if (fd == INVALID_SOCKET)
    return;

  epoll_event ev = { 0,{ 0 } };
  ev.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLPRI | EPOLLET;
  int result = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
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
        delete it->second.front();
        it->second.pop_front();
      }
      taskQueue[i].erase(it);
    }
  }
  epoll_event ev = { 0,{ 0 } };
  epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
}

bool Reactor::HasRegisteredOperation(int type, ReactorOperation* op)
{
  if (!(type < ReactorOps::max_ops))
    return false;

  auto it = taskQueue[type].find(op->descriptor);
  return it != std::end(taskQueue[type]);
}

void Reactor::Run(int timeout)
{
  std::error_code ec;
  epoll_event events[128];
  static const int flag[ReactorOps::max_ops] = { EPOLLIN, EPOLLOUT, EPOLLPRI };
  std::vector<std::pair<uint32_t, ReactorOperation*>> readyOps;
  ReactorOpsCleanup cleanupOps;

  int num_events = epoll_wait(epoll_fd, events, 128, timeout);

  for (int i = 0; i < num_events; ++i)
  {
    if (events[i].events & (EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLERR | EPOLLHUP))
    {
      void* ptr = events[i].data.ptr;
      auto op = static_cast<ReactorOperation*>(ptr);
      readyOps.push_back(std::make_pair(events[i].events, op));
    }
  }

  for (int i = ReactorOps::max_ops - 1; i >= 0; --i)
    for (std::size_t j = 0; j < readyOps.size(); ++j)
    {
      if (readyOps[j].first & (flag[i] | EPOLLERR | EPOLLHUP))
      {
        auto it = taskQueue[i].find(readyOps[j].second->descriptor);
        if (it != std::end(taskQueue[i]))
        {
          while (!it->second.empty())
          {
            auto rOp = it->second.front();
            it->second.pop_front();
            rOp->RunOperation(ec);
            cleanupOps.cl.push_back(rOp);
          } 
          taskQueue[i].erase(it);
        }
      }
    }
} 

}
#endif

