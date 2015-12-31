#pragma once

#include "resource.h"

namespace NetZ
{
namespace Http
{
  struct HttpMessageRequest;
  struct HttpMessageResponse;

  class ResourceManager
  {
  public:
    ResourceManager(const std::string& docRoot);
    
    bool GetResource(const HttpMessageRequest& request, HttpMessageResponse& response);
    bool AddResource(const HttpMessageRequest& request, HttpMessageResponse& response);
    InputBuffer ToReplyBuffer(HttpMessageResponse& response) const;
  private:
    std::string documentRoot;
    std::unique_ptr<Resource> selectedResource;
  };
}
}