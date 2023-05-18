#include <optional>

#include "msbt/tags.h"

namespace oepd::msbt::tags {

std::string hex_str(tcb::span<const u8> data) {
  size_t size = data.size();
  std::string s(size * 2, ' ');
  for (size_t i = 0; i < size; ++i) {
    s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
    s[2 * i + 1] = hexmap[data[i] & 0x0F];
  }

  return s;
}  // namespace oepd::msbt::tags

Tag* FillTag(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  Tag* tag;
  if (group_id == 201) {
    tag = new UnknownTag;
  } else {
    tag = new UnknownTag;
  }

  tag->Fill(group_id, type_id, data);
  return tag;
}

void UnknownTag::Fill(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  m_group = group_id;
  m_type = type_id;
  m_data = data;
}

void UnknownTag::Fill(std::string text) {}

std::string UnknownTag::ToText() {
  return "<" + std::to_string(m_group) + " Type='" + std::to_string(m_type) +
         (m_data.size() > 0 ? "' Data='" + hex_str(m_data) + "'/>" : "'/>");
}

std::vector<u8> UnknownTag::ToBinary() {}

}  // namespace oepd::msbt::tags