#ifndef UTILS_H
#define UTILS_H

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
  ec = std::error_code(errno, std::system_category());
  return return_value;
}

inline void DebugMessage(const char* format, ...)
{
#ifdef DEBUG
  va_list args;
  va_start(args, format);
  std::vector<char> buf(1+std::vsnprintf(nullptr, 0, format, args));
  va_end(args);
  std::vsnprintf(buf.data(), buf.size(), format, args);
  std::printf("%s\n", buf.data());
#endif
}

#endif
