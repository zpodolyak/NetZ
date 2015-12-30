#include "libraries/common.h"
#include "libraries/network/include/network.h"
#include "http_common.h"
#include "http_parser.h"

namespace NetZ
{
namespace Http
{
  namespace HttpParser
  {
    namespace
    {
      const char crlf[] = { '\r', '\n' };
      constexpr int http_version_string_length = 10;

      bool ReadUntilIf(const char*& from, const char* to, char delimiter, bool(*conditionFunc)(unsigned char))
      {
        while (*from != ' ' && from < to)
        {
          unsigned char c = *from++;
          if (conditionFunc(c))
            continue;
          --from;
          return false;
        }
        return true;
      }

      bool CheckCRLF(const char* from, const char* to)
      {
        return (from < to) ? *from == crlf[0] && *(from + 1) == crlf[1] : false;
      }
    }

    bool ParseRequestLine(InputBuffer& buffer, HttpMessageRequest& request)
    {
      const char* p = buffer.offset;
      const char* end = buffer.End();
      if (ReadUntilIf(p, end, ' ', [](unsigned char c) { return ('A' <= c && c <= 'Z'); }))
      {
        if (p < end)
          request.method.assign(buffer.offset, p++ - buffer.offset);
        else
        {
          buffer.offset = p;
          return false;
        }
      }
      else
      {
        buffer.sc = HttpStatusCode::bad_request;
        return false;
      }
      
      buffer.offset = p;
      if (ReadUntilIf(p, end, ' ', [](unsigned char c) { return !(c >= 0 && c <= 31) || (c == 127); }))
      {
        if (p < end)
          request.uri.assign(buffer.offset, p++ - buffer.offset);
        else
        {
          buffer.offset = p;
          return false;
        }
      }
      else
      {
        buffer.sc = HttpStatusCode::bad_request;
        return false;
      }

      if (http_version_string_length <= end - p)
      {
        if (*p != 'H' && *(p + 1) != 'T' && *(p + 2) != 'T' && *(p + 3) != 'T' && *(p + 4) != 'P' && *(p + 5) != '/')
        {
          buffer.sc = HttpStatusCode::bad_request;
          return false;
        }

        p += 5;
        unsigned char c = *p++;
        if (c == '1')
          request.versionMajor = c - '0';

        if (*p++ == '.')
        {
          if (*p == '0' || *p == '1')
            request.versionMinor = *p - '0';
          ++p;
        }

        if (request.versionMajor == -1 || request.versionMinor == -1 || *p != crlf[0] || *(p + 1) != crlf[1])
        {
          buffer.sc = HttpStatusCode::bad_request;
          return false;
        }
        buffer.offset = p + 2;
      }
      else
      {
        buffer.offset = p;
        return false;
      }
      return true;
    }

    bool ParseNextHeader(InputBuffer& buffer, HttpMessage& message)
    {
      Header header;
      const char* p = buffer.offset;
      const char* end = buffer.End();
      if (CheckCRLF(p, end)) return false;
      
      while (p < end && 32 < *p && *p != 127) ++p;
      if (p == end)
      {
        buffer.offset = p;
        return false;
      }

      if (*(p - 1) != ':')
      {
        buffer.sc = HttpStatusCode::bad_request;
        return false;
      }
      header.first.assign(buffer.offset, p - buffer.offset);

      while (p < end && (*p == ' ' || *p == '\t')) ++p;
      if (p == end)
      {
        buffer.offset = p;
        return false;
      }
      const char* start = p;
      while (p < end && ((31 < *p && *p != 127) || *p == '\t')) ++p;
      if (p == end)
      {
        buffer.offset = p;
        return false;
      }
      const char* ws = p - 1;
      while (*ws == ' ' || *ws == '\t') --ws;
      ++ws;
      header.second.assign(start, ws - start);

      if (!CheckCRLF(p, end))
      {
        buffer.sc = HttpStatusCode::bad_request;
        return false;
      }

      buffer.offset = p + 2;
      message.headers.push_back(std::move(header));
      return true;
    }
  }
}
}
