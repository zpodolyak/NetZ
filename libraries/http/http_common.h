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

    std::string GetStatusString() const
    {
      switch (statusCode)
      {
      case HttpStatusCode::ok: return "HTTP/1.1 200 OK\r\n";
      case HttpStatusCode::created: return "HTTP/1.1 201 Created\r\n";
      case HttpStatusCode::accepted: return "HTTP/1.1 202 Accepted\r\n";
      case HttpStatusCode::no_content: return "HTTP/1.1 204 No Content\r\n";
      case HttpStatusCode::multiple_choices: return "HTTP/1.1 300 Multiple Choices\r\n";
      case HttpStatusCode::moved_permanently: return "HTTP/1.1 301 Moved Permanently\r\n";
      case HttpStatusCode::moved_temporarily: return "HTTP/1.1 302 Moved Temporarily\r\n";
      case HttpStatusCode::not_modified: return "HTTP/1.1 304 Not Modified\r\n";
      case HttpStatusCode::bad_request: return "HTTP/1.1 400 Bad Request\r\n";
      case HttpStatusCode::unauthorized: return "HTTP/1.1 401 Unauthorized\r\n";
      case HttpStatusCode::forbidden: return "HTTP/1.1 403 Forbidden\r\n";
      case HttpStatusCode::not_found: return "HTTP/1.1 404 Not Found\r\n";
      case HttpStatusCode::internal_server_error: return "HTTP/1.1 500 Internal Server Error\r\n";
      case HttpStatusCode::not_implemented: return "HTTP/1.1 501 Not Implemented\r\n";
      case HttpStatusCode::bad_gateway: return "HTTP/1.1 502 Bad Gateway\r\n";
      case HttpStatusCode::service_unavailable: return "HTTP/1.1 503 Service Unavailable\r\n";
      }
      return "";
    }

    std::string GetDefaultReply() const;
  };
}
}