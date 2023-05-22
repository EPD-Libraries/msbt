#pragma once

#include <charconv>
#include <exio/types.h>
#include <msbt/tags.h>
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

static std::vector<u8>* from_hex(std::string_view hex) {
  auto data = new std::vector<u8>;
  for (size_t i = 0; i < hex.length(); i += 2) {
    u8 byte;
    std::string_view byte_char = hex.substr(i, 2);
    std::from_chars(byte_char.data(), byte_char.data() + byte_char.size(), byte, 16);
    data->push_back(byte);
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

static tags::TagParams parse_tag_params(std::string_view src, size_t pos) {
  tags::TagParams result;

  while (pos < src.length() - 1) {
    std::pair<std::string, std::string> entry;

    while (src[pos + 1] == ' ') {
      pos++;
    }

    while (src[++pos] != '=') {
      entry.first += src[pos];
    }

    if (src[++pos] == '\'') {
      while (src[++pos] != '\'') {
        entry.second += src[pos];
      }

      result.push_back(entry);
    }
  }

  return result;
}

static std::pair<std::string, tags::TagParams> parse_tag(std::string_view text) {
  std::pair<std::string, tags::TagParams> result;
  size_t pos = 0;

  while (text[pos] != ' ') {
    result.first += text[pos++];
  }

  result.second = parse_tag_params(text, pos);
  return result;
}

}  // namespace oepd::msbt::util
