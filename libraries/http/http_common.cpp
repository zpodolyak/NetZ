#include "http_common.h"

namespace NetZ
{
namespace Http
{
  namespace
  {
    const char* Code2String(HttpStatusCode code)
    {
      switch (code)
      {
      case HttpStatusCode::ok: return "OK";
      case HttpStatusCode::created: return "Created";
      case HttpStatusCode::accepted: return "Accepted";
      case HttpStatusCode::no_content: return "No Content";
      case HttpStatusCode::multiple_choices: return "Multiple Choices";
      case HttpStatusCode::moved_permanently: return "Moved Permanently";
      case HttpStatusCode::moved_temporarily: return "Moved Temporarily";
      case HttpStatusCode::not_modified: return "Not Modified";
      case HttpStatusCode::bad_request: return "Bad Request";
      case HttpStatusCode::unauthorized: return "Unauthorized";
      case HttpStatusCode::forbidden: return "Forbidden";
      case HttpStatusCode::not_found: return "Not Found";
      case HttpStatusCode::internal_server_error: return "Internal Server Error";
      case HttpStatusCode::not_implemented: return "Not Implemented";
      case HttpStatusCode::bad_gateway: return "Bad Gateway";
      case HttpStatusCode::service_unavailable: return "Service Unavailable";
      }
      return "";
    }

    std::string GetDefaultHtmlReply(HttpStatusCode code)
    {
      char reply[1024];
      const char* codeStr = Code2String(code);
      std::snprintf(reply, 1024, "<html><head><title>%s</title></head><body><h1>%d %s</h1></body></html>", codeStr, code, codeStr);
      return reply;
    }
  }

  std::string HttpMessageResponse::GetDefaultReply() const
  {
    std::string replyStr(GetStatusString());
    replyStr.append("Content-Length: ");
    auto defaultReply = GetDefaultHtmlReply(statusCode);
    replyStr.append(std::to_string(defaultReply.length()));
    replyStr.append("Content-Type: text/html");
    replyStr.append(defaultReply);
    return replyStr;
  }
}
}
