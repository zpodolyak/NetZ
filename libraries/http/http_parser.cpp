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
      bool ReadUntilIf(const char*& from, char delimiter, bool(*conditionFunc)(unsigned char))
      {
        while (*from != ' ')
        {
          unsigned char c = *from++;
          if (conditionFunc(c))
            continue;
          --from;
          return false;
        }
        return true;
      }

      const char crlf[] = { '\r', '\n' };
    }

    void ParseRequest(InputBuffer& buffer, HttpMessageRequest& request)
    {
      const char* p = buffer.offset;
      if (ReadUntilIf(p, ' ', [](unsigned char c) { return ('A' <= c && c <= 'Z'); }))
        request.method.assign(buffer.offset, p++ - buffer.offset);
      else
      {
        buffer.sc = HttpStatusCode::bad_request;
        return;
      }
      
      buffer.offset = p;
      if (ReadUntilIf(p, ' ', [](unsigned char c) { return !(c >= 0 && c <= 31) || (c == 127); }))
        request.uri.assign(buffer.offset, p++ - buffer.offset);
      else
      {
        buffer.sc = HttpStatusCode::bad_request;
        return;
      }

      if (*p != 'H' && *(p+1) != 'T' && *(p+2) != 'T' && *(p+3) != 'T' && *(p+4) != 'P' && *(p+5) != '/')
      {
        buffer.sc = HttpStatusCode::bad_request;
        return;
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

      if (request.versionMajor == -1 || request.versionMinor == -1 || *p != crlf[0] || *(p+1) != crlf[1])
      {
        buffer.sc = HttpStatusCode::bad_request;
        return;
      }

      buffer.offset = p+2;
      Header header;
      while (buffer.offset != buffer.End() && ParseNextHeader(buffer, header))
        request.headers.push_back(header);
    }

    bool ParseNextHeader(InputBuffer& buffer, Header& header)
    {
      const char* p = buffer.offset;
      if (*p == crlf[0] && *(p+1) == crlf[1])
        return false;
      
      while (32 < *p && *p != 127) ++p;
      if (*(p - 1) != ':')
      {
        buffer.sc = HttpStatusCode::bad_request;
        return false;
      }
      header.first.assign(buffer.offset, p - buffer.offset);

      while (*p == ' ' || *p == '\t') ++p;
      const char* start = p;
      while ((31 < *p && *p != 127) || *p == '\t') ++p;
      header.second.assign(start, p - start);

      if (*p != crlf[0] && *(p+1) != crlf[1])
      {
        buffer.sc = HttpStatusCode::bad_request;
        return false;
      }
      buffer.offset = p+2;
      return true;
    }
  }
}
}
