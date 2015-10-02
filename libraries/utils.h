#pragma once

inline void PrintError(const char* message, int error_code = 0)
{
  fprintf(stderr, "errno %s: %s\n", std::strerror((error_code != 0) ? error_code : errno), message);
}

inline void PrintError(const std::error_code& ec)
{
  std::stringstream ss;
  ss << ec;
  PrintError(ss.str().c_str());
}

inline int ErrorWrapper(int return_value, std::error_code& ec)
{
#ifdef WIN32
  ec = std::error_code(WSAGetLastError(), std::system_category());
#else
  ec = std::error_code(errno, std::system_category());
#endif
  return return_value;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"

template <typename... Targs>
inline void DebugMessage(const char* format, Targs&&... args)
{
#ifdef DEBUG
  int sz = std::snprintf(nullptr, 0, format, std::forward<Targs>(args)...);
  std::vector<char> buff(sz + 1);
  std::snprintf(&buff[0], buff.size(), format, std::forward<Targs>(args)...);
  std::printf("%s\n", buff.data());
#endif
}

#pragma clang diagnostic pop
