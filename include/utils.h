#ifndef UTILS_H
#define UTILS_H

template<typename T>
inline std::string to_string(T val) { return static_cast<std::ostringstream*>(&(std::ostringstream()<<val))->str(); }

#endif
