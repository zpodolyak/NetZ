#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "resource_manager.h"

namespace NetZ
{
namespace Http
{
  ResourceManager::ResourceManager()
  {
  }

  bool ResourceManager::FindHttpResource(const HttpMessageRequest& request, HttpMessageResponse& response)
  {
    return false;
  }

  Resource* ResourceManager::GetResource() const
  {
    return nullptr;
  }
}
}
