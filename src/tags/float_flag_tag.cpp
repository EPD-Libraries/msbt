#include "msbt/tags.h"
#include "util.h"

namespace oepd::msbt::tags {

void FloatFlagTag::Fill(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  auto reader = exio::BinaryReader{data, exio::Endianness::Little};
  u16 flag_name_len = *reader.Read<u16>();
  m_flag_name = reader.ReadWString(sizeof(u16), flag_name_len / 2);
  
  reader.Seek(sizeof(u16) + flag_name_len);
  m_unknown_1 = *reader.Read<s16>();
  m_unknown_2 = *reader.Read<s16>();
}

void FloatFlagTag::Fill(std::string_view group_name, TagParams params) {
  for (const auto& entry : params) {
    if (entry.first == "Name") {
      m_flag_name = util::convert_to_utf16(entry.second);
    }
    else if (entry.first == "Ukn1") {
      std::from_chars(entry.second.data(), entry.second.data() + entry.second.size(), m_unknown_1);
    }
    else if (entry.first == "Ukn2") {
      std::from_chars(entry.second.data(), entry.second.data() + entry.second.size(), m_unknown_2);
    }
  }
}

std::string FloatFlagTag::ToText() {
  return "<FloatFlag Name='" + util::convert_to_utf8(m_flag_name) +
    "' Unk1='" + std::to_string(m_unknown_1) +
    "' Unk2='" + std::to_string(m_unknown_2) +
    "'/>";
}

void FloatFlagTag::ToBinary(exio::BinaryWriter& writer) {
  writer.Write<u16>(2);
  writer.Write<u16>(9);
  writer.Write<u16>(
    sizeof(u16) + m_flag_name.size() +
    sizeof(u16) + sizeof(u16));

  writer.Write<u16>(m_flag_name.size());
  for (const wchar_t wchar : m_flag_name) {
    writer.Write(wchar);
  }
  
  writer.Write(m_unknown_1);
  writer.Write(m_unknown_2);
}

}  // namespace oepd::msbt::tags