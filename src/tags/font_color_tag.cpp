#include <unordered_map>
#include "msbt/tags.h"
#include "util.h"

namespace oepd::msbt::tags {

static std::unordered_map<std::string, FontColorTag::FontColor> color_to_enum_map = {
    {"White", FontColorTag::FontColor::White}, {"Red", FontColorTag::FontColor::Red}
};

static std::unordered_map<FontColorTag::FontColor, std::string> color_to_str_map = {
    {FontColorTag::FontColor::White, "White"}, {FontColorTag::FontColor::Red, "Red"}
};

void FontColorTag::Fill(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  m_font_color = *reinterpret_cast<const FontColorTag::FontColor*>(data.data());
}

void FontColorTag::Fill(std::string_view group_name, TagParams params) {
  for (const auto& entry : params) {
    if (entry.first == "Colour") {
      m_font_color = color_to_enum_map[entry.second];
    }
  }
}

std::string FontColorTag::ToText() {
  return "<FontColor Colour='" + color_to_str_map[m_font_color] + "'/>";
}

void FontColorTag::ToBinary(exio::BinaryWriter& writer) {
  writer.Write<u16>(0);
  writer.Write<u16>(3);
  writer.Write<u16>(2);
  writer.Write(m_font_color);
}

}  // namespace oepd::msbt::tags