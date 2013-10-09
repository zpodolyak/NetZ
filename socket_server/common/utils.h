#ifndef UTILS_H
#define UTILS_H

template<typename T>
inline std::string to_string(T val) { return static_cast<ostringstream*>(&(ostringstream()<<val))->str(); }

#endif
