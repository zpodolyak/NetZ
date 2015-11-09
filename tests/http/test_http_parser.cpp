#include <gtest/gtest.h>

#include "libraries/common.h"
#include "libraries/http/http_common.h"
#include "libraries/http/http_parser.h"

using NetZ::Http::InputBuffer;
using NetZ::Http::HttpMessageRequest;
using NetZ::Http::HttpStatusCode;
using NetZ::Http::Header;

TEST(TestParseRequest, ParseRequestComplete)
{
  HttpMessageRequest req;
  const char* inputStr = "GET /http_resource.html HTTP/1.1\r\n"
                         "User-Agent: curl / 7.16.3 libcurl / 7.16.3 OpenSSL / 0.9.7l zlib / 1.2.3\r\n"
                         "Host: www.example.com\r\n";
  InputBuffer input(inputStr, strlen(inputStr));
  EXPECT_TRUE(NetZ::Http::HttpParser::ParseRequestLine(input, req));
  EXPECT_EQ(req.method, "GET");
  EXPECT_EQ(req.uri, "/http_resource.html");
  EXPECT_EQ(req.versionMajor, 1);
  EXPECT_EQ(req.versionMinor, 1);
  while (input.offset != input.End() && NetZ::Http::HttpParser::ParseNextHeader(input, req));
  EXPECT_EQ(req.headers.size(), 2);
}

TEST(TestParseHeader, ParseHeaderComplete)
{
  HttpMessageRequest req;
  const char* inputStr = "Host : www.example.com\r\n";
  InputBuffer input(inputStr, strlen(inputStr));
  EXPECT_FALSE(NetZ::Http::HttpParser::ParseNextHeader(input, req));
  EXPECT_EQ(input.sc, HttpStatusCode::bad_request);
  inputStr = "\r\n";
  input.SetBuffer(inputStr, strlen(inputStr));
  EXPECT_FALSE(NetZ::Http::HttpParser::ParseNextHeader(input, req));
  EXPECT_EQ(input.sc, HttpStatusCode::bad_request);
  inputStr = " Host: www.example.com\r\n";
  input.SetBuffer(inputStr, strlen(inputStr));
  EXPECT_FALSE(NetZ::Http::HttpParser::ParseNextHeader(input, req));
  EXPECT_EQ(input.sc, HttpStatusCode::bad_request);
  inputStr = "Host: www.example.com\n";
  input.SetBuffer(inputStr, strlen(inputStr));
  EXPECT_FALSE(NetZ::Http::HttpParser::ParseNextHeader(input, req));
  EXPECT_EQ(input.sc, HttpStatusCode::bad_request);
  inputStr = "Host:      www.example.com\t\t    some text       \t\t       \r\n";
  input.SetBuffer(inputStr, strlen(inputStr));
  EXPECT_TRUE(NetZ::Http::HttpParser::ParseNextHeader(input, req));
  EXPECT_EQ(req.headers[0].first, "Host:");
  EXPECT_EQ(req.headers[0].second, "www.example.com\t\t    some text");
  EXPECT_EQ(input.sc, HttpStatusCode::ok);
}

TEST(TestParseRequestPartial, ParseRequestPartial)
{
  HttpMessageRequest req;
  const char* inputStr = "GET";
  InputBuffer input(inputStr, strlen(inputStr));
  EXPECT_FALSE(NetZ::Http::HttpParser::ParseRequestLine(input, req));
  EXPECT_EQ(input.sc, HttpStatusCode::ok);
  EXPECT_EQ(input.offset, input.End());
  inputStr = "GET /http_resour";
  input.SetBuffer(inputStr, strlen(inputStr));
  EXPECT_FALSE(NetZ::Http::HttpParser::ParseRequestLine(input, req));
  EXPECT_EQ(req.method, "GET");
  EXPECT_EQ(input.sc, HttpStatusCode::ok);
  EXPECT_EQ(input.offset, input.End());
  inputStr = "GET /http_resource.html HT";
  input.SetBuffer(inputStr, strlen(inputStr));
  EXPECT_FALSE(NetZ::Http::HttpParser::ParseRequestLine(input, req));
  EXPECT_EQ(req.method, "GET");
  EXPECT_EQ(req.uri, "/http_resource.html");
  EXPECT_EQ(input.sc, HttpStatusCode::ok);
  EXPECT_EQ(*input.offset, 'H');
}

TEST(TestParseHeaderPartial, ParseHeaderPartial)
{
  HttpMessageRequest req;
  const char* inputStr = "Hos";
  InputBuffer input(inputStr, strlen(inputStr));
  EXPECT_FALSE(NetZ::Http::HttpParser::ParseNextHeader(input, req));
  EXPECT_EQ(input.sc, HttpStatusCode::ok);
  EXPECT_EQ(input.offset, input.End());
  inputStr = "Host:     ";
  input.SetBuffer(inputStr, strlen(inputStr));
  EXPECT_FALSE(NetZ::Http::HttpParser::ParseNextHeader(input, req));
  EXPECT_EQ(input.sc, HttpStatusCode::ok);
  EXPECT_EQ(input.offset, input.End());
  inputStr = "Host: www.example.com";
  input.SetBuffer(inputStr, strlen(inputStr));
  EXPECT_FALSE(NetZ::Http::HttpParser::ParseNextHeader(input, req));
  EXPECT_EQ(input.sc, HttpStatusCode::ok);
  EXPECT_EQ(input.offset, input.End());
}

#include "libraries/http/http_parser.cpp"

