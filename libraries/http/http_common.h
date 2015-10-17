#pragma once

#include <string>
#include <utility>
#include <vector>

namespace NetZ
{
namespace Http
{
  using Header = std::pair<std::string, std::string>;

  struct HttpMessage
  {
    int versionMajor = -1;
    int versionMinor = -1;
    std::vector<Header> headers;
  };

  struct HttpMessageRequest : public HttpMessage
  {
    std::string method;
    std::string uri;
  };

  enum class HttpStatusCode
  {
    ok = 200,
    created = 201,
    accepted = 202,
    no_content = 204,
    multiple_choices = 300,
    moved_permanently = 301,
    moved_temporarily = 302,
    not_modified = 304,
    bad_request = 400,
    unauthorized = 401,
    forbidden = 403,
    not_found = 404,
    internal_server_error = 500,
    not_implemented = 501,
    bad_gateway = 502,
    service_unavailable = 503
  };


  struct HttpMessageResponse : public HttpMessage
  {
    HttpStatusCode statusCode;
  };
}
}