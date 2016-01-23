#include "libraries/common.h"
#include "network.h"

#ifndef HAS_EPOLL
# include "windows/select_reactor.h"

namespace NetZ
{
  Reactor::Reactor()
  {

  }

  Reactor::~Reactor()
  {
    Stop();
  }

  void Reactor::RegisterOperation(int type, ReactorOperation* op)
  {
    if (!(type < ReactorOps::max_ops) && op->descriptor == INVALID_SOCKET)
      return;

    auto it = taskQueue[type].emplace(std::piecewise_construct, std::forward_as_tuple(op->descriptor), std::forward_as_tuple());
    it.first->second.push_back(op);
  }
  
  void Reactor::RegisterDescriptor(SocketHandle fd)
  {

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
          // auto ec = std::make_error_code(std::errc::operation_canceled);
          delete op;
          it->second.pop_front();
        }
        taskQueue[i].erase(it);
      }
    }
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
    for (int i = 0; i < ReactorOps::max_ops; ++i)
    {
      FD_ZERO(&fds[i]);
      for(auto fd : taskQueue[i])
        FD_SET(fd.first, &fds[i]);
    }

    timeval tv_buf = { 0, 0 };
    auto result = ErrorWrapper(::select(0, &fds[ReactorOps::read],
      &fds[ReactorOps::write], &fds[ReactorOps::error], &tv_buf), ec);

    if (result > 0)
      for (int i = ReactorOps::max_ops - 1; i >= 0; --i)
        for (std::size_t j = 0; j < fds[i].fd_count; ++j)
        { 
          auto it = taskQueue[i].find(fds[i].fd_array[j]);
          if (it != std::end(taskQueue[i]))
          {
            while (!it->second.empty())
            {
              auto op = it->second.front();
              it->second.pop_front();
              op->RunOperation(ec);
              delete op;
            }
            taskQueue[i].erase(it);
          }
        }
  }

  void Reactor::Stop()
  {
    shutdown = true;
  }
}

#endif
