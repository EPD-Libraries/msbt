#include <unordered_map>
#include "msbt/tags.h"
#include "util.h"

namespace oepd::msbt::tags {

static std::unordered_map<std::string, FontTag::FontType> font_to_enum_map = {
    {"Normal", FontTag::FontType::Normal}, {"Unkown", FontTag::FontType::Unknown}, {"Hylian", FontTag::FontType::Hylian}};

static std::unordered_map<FontTag::FontType, std::string> font_to_str_map = {
    {FontTag::FontType::Normal, "Normal"}, {FontTag::FontType::Unknown, "Unkown"}, {FontTag::FontType::Hylian, "Hylian"}};

void FontTag::Fill(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  m_font = *reinterpret_cast<const FontTag::FontType*>(data.data());
}

void FontTag::Fill(std::string_view group_name, TagParams params) {
  for (const auto entry : params) {
    if (entry.first == "Font") {
      m_font = font_to_enum_map[entry.second];
    }
  }
}

std::string FontTag::ToText() {
  return "<Font Type='" + font_to_str_map[m_font] + "'/>";
}

void FontTag::ToBinary(exio::BinaryWriter& writer) {
  writer.Write<u16>(0);
  writer.Write<u16>(0);
  writer.Write<u16>(2);
  writer.Write(m_font);
}

}  // namespace oepd::msbt::tags
