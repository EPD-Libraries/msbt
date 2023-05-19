#pragma once

#include <exio/types.h>
#include <nonstd/span.h>
#include <string>
#include <vector>

namespace oepd::msbt::util {

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

static std::string to_hex(tcb::span<const u8> data) {
  size_t size = data.size();
  std::string s(size * 2, ' ');
  for (size_t i = 0; i < size; ++i) {
    s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
    s[2 * i + 1] = hexmap[data[i] & 0x0F];
  }

  return s;
}

static std::vector<u8> from_hex(std::string hex) {
  std::vector<u8> data;

  for (size_t i = 0; i < hex.length(); i += 2) {
    std::string byte = hex.substr(i, 2);
    data.push_back(strtol(byte.c_str(), NULL, 16));
  }

  return data;
}

static inline void replace_all(std::string& src, const std::string& from, const std::string& to) {
  size_t start_pos = 0;
  while ((start_pos = src.find(from, start_pos)) != std::string::npos) {
    src.replace(start_pos, from.length(), to);
    start_pos += to.length();  // Handles case where 'to' is a substring of 'from'
  }
}

static std::vector<std::pair<std::string, std::string>> parse_params(std::string src, size_t pos) {
  std::vector<std::pair<std::string, std::string>> result;

  while (pos < src.length() || src[pos] != ' ') {
    std::pair<std::string, std::string> entry;

    while (src[++pos] != '\'') {
      entry.first += src[pos];
    }

    while (src[++pos] != '\'') {
      entry.second += src[pos];
    }

    result.push_back(entry);
  }
}

}  // namespace oepd::msbt::util
