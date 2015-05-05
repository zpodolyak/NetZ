#ifndef UTILS_H
#define UTILS_H

inline void PrintError(const char *message)
{
  fprintf(stderr, "%s error: %s\n", std::strerror(errno), message);
}

#endif
