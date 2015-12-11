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
      return selectedResource->Load();
    }
    return false;
  }

  bool ResourceManager::AddResource(const HttpMessageRequest& request, HttpMessageResponse& response)
  {
    return false;
  }

  Resource* ResourceManager::ToResource() const
  {
    return selectedResource.get();
  }
}
}
