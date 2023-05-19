#pragma once

#include <string>

namespace oepd::msbt::util {

static inline void replace_all(std::string& src, const std::string& from, const std::string& to) {
  size_t start_pos = 0;
  while ((start_pos = src.find(from, start_pos)) != std::string::npos) {
    src.replace(start_pos, from.length(), to);
    start_pos += to.length();  // Handles case where 'to' is a substring of 'from'
  }
}

}  // namespace oepd::msbt::util
