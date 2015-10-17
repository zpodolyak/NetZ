#pragma once

namespace NetZ
{
namespace Http
{
  struct InputBuffer
  {
    using Storage = std::vector<char>;
    Storage buffer;
    HttpStatusCode sc;
    const char* offset;

    InputBuffer(const char* bufferData, std::size_t length)
      : buffer(bufferData, bufferData + length)
      , offset(Start())
      , sc(HttpStatusCode::ok)
    {
    }

    const char* Start() { return buffer.data(); }
    const char* End() { return buffer.data() + buffer.size(); }

    explicit operator std::string() const
    {
      return std::string(buffer.data(), buffer.size());
    }
  };

  namespace HttpParser
  {
    void ParseRequest(InputBuffer& buffer, HttpMessageRequest& request);
    bool ParseNextHeader(InputBuffer& buffer, Header& header);
  };
}
}
