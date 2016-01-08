#include "libraries/common.h"
#include "http_common.h"
#include "http_parser.h"
#include "resource.h"

namespace NetZ
{
namespace Http
{ 
  constexpr int read_size = 8192;

  FileResource::FileResource(std::string file)
    : rawData()
    , path(std::move(file))
  {
  }

  bool FileResource::Load()
  {
    std::ifstream is(path.c_str(), std::ios::in);
    DebugMessage("loading %s", path.c_str());
    if (is)
    {
      char buffer[read_size];
      while (is.read(buffer, sizeof(buffer)).gcount() > 0)
        rawData.insert(std::end(rawData), buffer, buffer + is.gcount());
      is.close();
      return true;
    }
    return false;
  }

  InputBuffer FileResource::ToBuffer() const
  {
    return InputBuffer(rawData.data(), rawData.size());
  }
}
}
