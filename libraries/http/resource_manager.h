#pragma once

namespace NetZ
{
namespace Http
{
  struct HttpMessageRequest;
  struct HttpMessageResponse;

  class ResourceManager
  {
  public:
    ResourceManager();
    
    bool FindHttpResource(const HttpMessageRequest& request, HttpMessageResponse& response);
  };
}
}