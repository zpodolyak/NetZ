#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "http_common.h"
#include "resource_manager.h"

namespace NetZ
{
namespace Http
{
  ResourceManager::ResourceManager(const std::string& docRoot)
    : documentRoot(docRoot)
  {
  }

  bool ResourceManager::GetResource(const HttpMessageRequest& request, HttpMessageResponse& response)
  {
    if (request.uri.empty() || request.uri[0] != '/')
      return false;

    auto fullPath = documentRoot + request.uri;
    if (request.uri[request.uri.size() - 1] == '/')
    {
      fullPath += "index.html";
    }

    auto lastSlashPos = fullPath.find_last_of("/");
    auto lastDotPos = fullPath.find_last_of('.');
    if (lastDotPos != std::string::npos && lastSlashPos < lastDotPos)
    {
      selectedResource = make_unique<FileResource>(fullPath);
      if (selectedResource->Load())
      {
        response.statusCode = HttpStatusCode::ok;
        return true;
      }
    }
    return false;
  }

  bool ResourceManager::AddResource(const HttpMessageRequest& request, HttpMessageResponse& response)
  {
    return false;
  }

  InputBuffer ResourceManager::ToReplyBuffer(HttpMessageResponse& response) const
  {
    InputBuffer replyBuff;
    if (selectedResource)
    {
      replyBuff.Append(response.GetStatusString());
      std::string headerString("Content-Length: ");
      headerString.append(std::to_string(selectedResource->Size()));
      headerString.append("\r\n");
      headerString.append("Content-Type: text/html\r\n\r\n");
      replyBuff.Append(std::move(headerString));
      replyBuff.Append(selectedResource->ToBuffer().Start(), selectedResource->Size());
    }
    return replyBuff;
  }
}
}
