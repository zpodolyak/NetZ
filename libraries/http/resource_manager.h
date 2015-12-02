#pragma once

namespace NetZ
{
namespace Http
{
  struct HttpMessageRequest;
  struct HttpMessageResponse;
  class Resource;

  class ResourceManager
  {
  public:
    ResourceManager();
    
    bool FindHttpResource(const HttpMessageRequest& request, HttpMessageResponse& response);
    Resource* GetResource() const;
  };
}
}