#include "msbt/tags.h"

namespace oepd::msbt::tags {

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

std::string hex_str(tcb::span<const u8> data) {
  size_t size = data.size();
  std::string s(size * 2, ' ');
  for (size_t i = 0; i < size; ++i) {
    s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
    s[2 * i + 1] = hexmap[data[i] & 0x0F];
  }

  return s;
}

std::string GetText(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  // All tag types will be identified here
  // and sent to a tag class for processing
  return '<' + std::to_string(group_id) + " Type='" + std::to_string(type_id) +
         (data.size() > 0 ? "' Data='" + hex_str(data) + "'/>" : "'/>");
}

tcb::span<const u8> GetBinary(std::string tag) {
  return {};
}

}  // namespace oepd::msbt::tags