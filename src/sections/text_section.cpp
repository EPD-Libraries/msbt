#include <codecvt>
#include <locale>
#include <string>

#include "msbt/msbt.h"
#include "msbt/tags.h"
#include "util.h"

namespace oepd::msbt {

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;

TextSection::TextEntryValue::TextEntryValue(tags::Tag* tag) {
  m_tag = tag;
}

TextSection::TextEntryValue::TextEntryValue(std::wstring text) {
  m_text = text;
}

void TextSection::TextEntry::Fill(tcb::span<const u8> data) {
  exio::BinaryReader reader{data, exio::Endianness::Little};

  size_t size = data.size() / sizeof(wchar_t);
  std::wstring utf_16{reinterpret_cast<const wchar_t*>(data.begin()), size};

  for (size_t i = 0; i < utf_16.size(); i++) {
    wchar_t wchar = utf_16[i];

    if (wchar == 0x0E) {
      const auto group_id = *reader.Read<u16>(++i * 2);
      const auto type_id = *reader.Read<u16>(++i * 2);
      const auto data_len = *reader.Read<u16>(++i * 2);

      auto tag = tags::FillTag(group_id, type_id, data.subspan(++i * 2, data_len));
      --i += data_len / sizeof(wchar_t);
      m_values.push_back(TextEntryValue{tag});
    } else if (wchar == 0x0F) {
      throw exio::UnsupportedError("Region tags are unsupported");
    } else if (wchar == 0x00) {
      // ignore null
    } else {
      if (m_values.empty() || !m_values.back().m_text) {
        m_values.push_back(TextEntryValue{std::wstring{}});
      }

      *m_values.back().m_text += wchar;
    }
  }
}

TextSection::TextEntry TextSection::TextEntry::FromText(std::string text) {}

std::string TextSection::TextEntry::ToText(size_t indent_level, bool one_line) {
  const auto indentation = std::string(indent_level, ' ');
  std::string result = indentation;
  for (auto value : m_values) {
    if (value.m_tag != nullptr) {
      result += value.m_tag->ToText();
    } else {
      std::string utf8 = converter.to_bytes(*value.m_text);
      util::replace_all(utf8, "\n", "\n" + indentation);
      result += utf8;
    }
  }

  return result;
}

TextSection::TextSection(exio::BinaryReader& reader, size_t table_size) {
  const auto text_table_offset = reader.Tell();
  const auto table = *reader.Read<SectionTable>();
  m_text_entries = std::vector<TextSection::TextEntry>{table.offset_count};

  for (size_t i = 0; i < table.offset_count; i++) {
    const auto offset_ptr = text_table_offset + sizeof(SectionTable) + sizeof(u32) * i;
    const auto offset = *reader.Read<u32>(offset_ptr);
    auto next_offset = *reader.Read<u32>(offset_ptr + sizeof(u32));

    m_text_entries[i].Fill(
        reader.span().subspan(text_table_offset + offset, (i == table.offset_count - 1 ? table_size : next_offset) - offset));
  }

  reader.Seek(table_size);
};

}  // namespace oepd::msbt