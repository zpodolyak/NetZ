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
      , sc(HttpStatusCode::ok)
      , offset(Start())
    {
    }

    const char* Start() { return buffer.data(); }
    const char* End() { return buffer.data() + buffer.size(); }

    explicit operator std::string() const
    {
      return std::string(buffer.data(), buffer.size());
    }

    explicit operator const char*() const
    {
      return buffer.data();
    }

    void Append(const char* bufferData, std::size_t length)
    {
      buffer.insert(std::end(buffer), bufferData, bufferData + length);
    }

    void SetBuffer(const char* bufferData, std::size_t length)
    {
      buffer.assign(bufferData, bufferData + length);
      Reset();
    }

    void Reset()
    {
      offset = Start();
      sc = HttpStatusCode::ok;
    }
  };

  namespace HttpParser
  {
    enum class ParseState
    {
      RequestParsing,
      HeaderParsing
    };

    bool ParseRequestLine(InputBuffer& buffer, HttpMessageRequest& request);
    bool ParseNextHeader(InputBuffer& buffer, HttpMessage& message);
  };
}
}
