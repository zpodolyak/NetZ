#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "libraries/common.h"
#include "libraries/http/http_common.h"
#include "libraries/http/http_parser.h"

using NetZ::Http::InputBuffer;
using NetZ::Http::HttpMessageRequest;
using NetZ::Http::HttpStatusCode;

TEST(TestParseRequest, ParseRequest)
{
  HttpMessageRequest req;
  const char* inputStr = "GET /http_resource.html HTTP/1.1\r\n"
                         "User-Agent: curl / 7.16.3 libcurl / 7.16.3 OpenSSL / 0.9.7l zlib / 1.2.3\r\n"
                         "Host: www.example.com\r\n";
  InputBuffer input(inputStr, strlen(inputStr));
  NetZ::Http::HttpParser::ParseRequest(input, req);
  EXPECT_EQ(req.method, "GET");
  EXPECT_EQ(req.uri, "/http_resource.html");
  EXPECT_EQ(req.versionMajor, 1);
  EXPECT_EQ(req.versionMinor, 1);
  EXPECT_EQ(req.headers.size(), 2);
}

TEST(TestParseHeader, ParseHeader)
{

}

#include "libraries/http/http_parser.cpp"

