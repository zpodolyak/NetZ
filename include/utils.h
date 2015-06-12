#ifndef UTILS_H
#define UTILS_H

inline void PrintError(const char *message)
{
  fprintf(stderr, "%s error: %s\n", std::strerror(errno), message);
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
