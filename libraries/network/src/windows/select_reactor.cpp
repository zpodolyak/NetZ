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

  void Reactor::RegisterDescriptorOperation(int type, ReactorOperation* op)
  {
    if (!(type < ReactorOps::max_ops) && op->descriptor == INVALID_SOCKET)
      return;

    auto it = taskQueue[type].emplace(std::piecewise_construct, std::forward_as_tuple(op->descriptor), std::forward_as_tuple());
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
          // auto ec = std::make_error_code(std::errc::operation_canceled);
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
          for (auto it = taskQueue[i][fds[i].fd_array[j]].begin(); it != taskQueue[i][fds[i].fd_array[j]].end();)
          {
            auto rOp = *it;
            rOp->RunOperation(ec);
            delete rOp;
            it = taskQueue[i][fds[i].fd_array[j]].erase(it);
          }
  }

  void Reactor::Stop()
  {
    shutdown = true;
  }
}

#endif
