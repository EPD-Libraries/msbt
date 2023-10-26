#include "msbt/tags.h"
#include "util.h"

namespace oepd::msbt::tags {

void FontSizeTag::Fill(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  m_font_size = *reinterpret_cast<const u16*>(data.data());
}

void FontSizeTag::Fill(std::string_view group_name, TagParams params) {
  for (const auto& entry : params) {
    if (entry.first == "Scale") {
      std::from_chars(entry.second.data(), entry.second.data() + entry.second.size(), m_font_size);
    }
  }
}

std::string FontSizeTag::ToText() {
  return "<FontSize Scale='" + std::to_string(m_font_size) + "'/>";
}

void FontSizeTag::ToBinary(exio::BinaryWriter& writer) {
  writer.Write<u16>(0);
  writer.Write<u16>(2);
  writer.Write<u16>(2);
  writer.Write(m_font_size);
}

}  // namespace oepd::msbt::tags
