#pragma once

#include "http_common.h"
#include "http_parser.h"

namespace NetZ
{
namespace Http
{
  class Resource
  {
  public:
    Resource() = default;
    virtual ~Resource() = default;
    virtual InputBuffer ToBuffer() const = 0;
  };
}
}
