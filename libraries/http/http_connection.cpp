#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "resource.h"
#include "resource_manager.h"
#include "http_connection.h"

namespace NetZ
{
namespace Http
{
  HttpConnection::HttpConnection(TcpSocket&& _socket, SocketService* _service, ResourceManager* rMgr)
    : socket(std::move(_socket))
    , resource_mgr(rMgr)
    , service(_service)
  {
  }

  void HttpConnection::Start()
  {
    Read(HttpParser::ParseState::RequestParsing);
  }

  void HttpConnection::Stop()
  {
    socket.Close();
  }

  void HttpConnection::Read(HttpParser::ParseState state)
  {
    socket.Receive(receiveBuffer, buffer_size, 0, [this, state](int bytes_transferred, const std::error_code& ec)
    {
      service->ResetTimer(socketTimeoutTimer);
      if (bytes_transferred > 0 && !ec)
      {
        InputBuffer input(receiveBuffer, bytes_transferred);
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
        while (HttpParser::ParseNextHeader(input, request))
        {
          if (input.sc != HttpStatusCode::ok)
          {
            response.statusCode = input.sc;
            WriteDefaultResponse();
            return;
          }
        }
        if (request.method == "GET")
        {
          if (!resource_mgr->GetResource(request, response))
          {
            response.statusCode = HttpStatusCode::not_found;
            WriteDefaultResponse();
          }
          else
          {
            reply = resource_mgr->ToResource()->ToBuffer();
            Write();
          }
        }
        else if (request.method == "POST")
        {
          resource_mgr->AddResource(request, response);
          reply = resource_mgr->ToResource()->ToBuffer();
          Write();
        }
      }
    });
  }

  void HttpConnection::Write()
  {
    socket.Send(static_cast<const char*>(reply), reply.buffer.size(), 0, [this](int bytes_transferred, const std::error_code& ec)
    {
      service->ResetTimer(socketTimeoutTimer);
      if (ec == std::errc::operation_canceled)
      {
        Stop();
      }
    });
  }

  void HttpConnection::WriteDefaultResponse()
  {
    auto replyString = response.GetDefaultReply();
    reply.Append(replyString.c_str(), replyString.length());
    Write();
  }
}
}
