#pragma once

namespace Netz
{
  enum ReactorOps
  {
    read = 0,
    write,
    error,
    max_ops,
    connect = write
  };

  const int REACTOR_QUEUES_SIZE = 3;

  using RunHandler = std::function<void(std::error_code&)>;
  using CompletionHandler = void(*)(const std::error_code&);
  using DataCompletionHandler = void(*)(int, const std::error_code&);
  template <typename SocketType>
  using AcceptCompletionHandler = void(*)(SocketType&, const std::error_code&);

  class ReactorOperation
  {
    friend class Reactor;
  public:
    ReactorOperation(SocketHandle fd, RunHandler rFunc)
      : descriptor(fd)
      , run(std::move(rFunc))
    {}

    void RunOperation(std::error_code& ec_)
    {
      run(ec_);
    }

    virtual void CompleteOperation() {}
  protected:
    SocketHandle descriptor;
    RunHandler run;
    std::error_code ec;
  };
}

#include "select_reactor.h"

