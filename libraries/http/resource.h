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
    virtual ~Resource() = default;
    virtual bool Load() = 0;
    virtual std::size_t Size() const = 0;
    virtual InputBuffer ToBuffer() const = 0;
  };

  class FileResource : public Resource
  {
  public:
    FileResource(std::string file);

    virtual bool Load() override;
    virtual std::size_t Size() const override 
    {
      return rawData.size();
    }
    virtual InputBuffer ToBuffer() const override;
  private:
    std::vector<char> rawData;
    std::string path;
  };
}
}
