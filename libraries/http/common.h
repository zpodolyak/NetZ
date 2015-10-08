#pragma once

#include <string>
#include <utility>
#include <vector>

namespace NetZ
{
namespace Http
{
  const char CRLF[] = { '\r', '\n' };
  const char SP = ' ';

  using Header = std::pair<std::string, std::string>;

  struct Request
  {
    std::string method;
    std::string uri;
    int versionMajor;
    int versionMinor;
    std::vector<Header> headers;
  };

  struct Response
  {
    int versionMajor;
    int versionMinor;
    int statusCode;
    std::string statusDesc;
  };
}
}