#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "resource_manager.h"
#include "http_connection.h"
#include "http_parser.h"

constexpr int buffer_size = 4096;

namespace NetZ
{
namespace Http
{
  HttpConnection::HttpConnection(TcpSocket&& _socket, ResourceManager* rMgr)
    : socket(std::move(_socket))
    , resource_mgr(rMgr)
  {
    Read(HttpParser::ParseState::RequestParsing);
  }

  void HttpConnection::Stop()
  {
    socket.Close();
  }

  void HttpConnection::Read(HttpParser::ParseState state)
  {
    char buffer[buffer_size];
    socket.Receive(buffer, buffer_size, 0, [this, &state, &buffer](int bytes_transferred, const std::error_code& ec)
    {
      if (socketTimeout) socketTimeout->Cancel();
      if (bytes_transferred > 0 && !ec)
      {
        InputBuffer input(buffer, bytes_transferred);
        if (state == HttpParser::ParseState::RequestParsing && !HttpParser::ParseRequestLine(input, request))
        {
          if (input.sc != HttpStatusCode::ok)
          {
            response.statusCode = input.sc;
            WriteDefaultResponse();
            return;
          }
          else Read(HttpParser::ParseState::RequestParsing);
        }
        while (input.offset != input.End())
        {
          if (!HttpParser::ParseNextHeader(input, request))
          {
            if (input.sc != HttpStatusCode::ok)
            {
              response.statusCode = input.sc;
              WriteDefaultResponse();
              return;
            }
            else Read(HttpParser::ParseState::HeaderParsing);
          }
        }
        if (!resource_mgr->FindHttpResource(request, response))
        {
          WriteDefaultResponse();
        }
      }
    });
  }

  void HttpConnection::Write(const InputBuffer& data)
  {
    socket.Send(static_cast<const char*>(data), data.buffer.size(), 0, [this](int bytes_transferred, const std::error_code& ec)
    {
      if (socketTimeout) socketTimeout->Reset();
      if (ec == std::errc::operation_canceled)
      {
        Stop();
      }
    });
  }

  void HttpConnection::WriteDefaultResponse()
  {
    auto replyString = resource_mgr->GetDefaultReply(response);
    InputBuffer data(replyString.c_str(), replyString.length());
    Write(data);
  }
}
}
