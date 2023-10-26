#include <codecvt>
#include <locale>
#include <string>

#include "msbt/msbt.h"

#pragma pack(push, 1)

namespace oepd::msbt {

struct Header {
  std::array<char, 8> magic;
  u16 bom;
  u16 _padding_1;
  u16 version;
  u16 num_sections;
  u16 _padding_2;
  u32 file_size;
  std::array<u8, 10> _padding_3;
  EXIO_DEFINE_FIELDS(Header, magic, bom, _padding_1, version, num_sections, _padding_2, file_size, _padding_3);
};
static_assert(sizeof(Header) == 0x20);

MSBT::MSBT(tcb::span<const u8> data) : m_reader{data, exio::Endianness::Little} {
  const auto header = *m_reader.Read<Header>();

  if (header.magic != MsbtMagic) {
    throw exio::InvalidDataError("Invalid MSBT magic");
  }
  if (header.version != 0x301) {
    throw exio::InvalidDataError("Only MSBT version 3.0.1 is supported");
  }

  for (size_t i = 0; i < header.num_sections; i++) {
    const auto table_header = *m_reader.Read<SectionHeader>();
    const auto magic = table_header.magic;
    if (magic == LabelSectionMagic) {
      m_label_section = LabelSection{m_reader};
    } /* else if (magic == AttributeSectionMagic) {
      m_attribute_section = AttributeSection{m_reader};
    } */
    else if (magic == TextSectionMagic) {
      m_text_section = TextSection{m_reader, table_header.table_size};
    } else {
      throw exio::InvalidDataError("Unsupported data block: " + std::string{magic.begin(), magic.end()});
    }

    m_reader.Seek(exio::util::AlignUp(m_reader.Tell(), 0x10));
  }

  if (!m_label_section) {
    throw exio::InvalidDataError("The label section (LBL1) was not found");
  } else if (!m_text_section) {
    throw exio::InvalidDataError("The text section (TXT2) was not found");
  }
}

MSBT::MSBT(std::string_view text) {
  m_label_section = LabelSection{};
  m_text_section = TextSection{};

  size_t index = 0;
  size_t pos = text.find(':');
  size_t i = 0;

  if (text.back() != '\n') {
    text = std::string(text) + '\n';
  }

  while (i < text.length()) {
    m_label_section->m_label_entries.push_back({index, std::string{text.substr(i, pos - i)}});

    std::string body;
    i = text.find("\n  ", pos) + 2;
    while (text[++i] != '\n' || text.substr(i + 1, 2) == "  ") {
      body += text[i];
      if (text[i] == '\n') {
        i += 2;
      }
    }

    m_text_section->m_text_entries.push_back(TextSection::TextEntry{body});

    pos = text.find(':', i++);
    index++;
  }
}

std::vector<u8> MSBT::ToBinary() {
  exio::BinaryWriter writer{exio::Endianness::Little};
  writer.Seek(sizeof(Header));

  size_t num_sections = 0;

  if (m_label_section) {
    m_label_section->Write(writer);
    writer.AlignUp(0x10);
    num_sections++;
  }

  if (m_attribute_section) {
    m_attribute_section->Write(writer);
    writer.AlignUp(0x10);
    num_sections++;
  }

  if (m_text_section) {
    m_text_section->Write(writer);
    writer.AlignUp(0x10);
    num_sections++;
  }

  writer.GrowBuffer();

  Header header;
  header.magic = MsbtMagic;
  header.bom = 0xFEFF;
  header._padding_1 = 0;
  header.version = 0x301;
  header.num_sections = num_sections;
  header._padding_2 = 0;
  header.file_size = writer.Buffer().size();
  header._padding_3.fill(0x00);

  writer.Seek(0);
  writer.Write(header);

  return writer.Finalize();
}

std::string MSBT::ToText() {
  std::string result;
  for (const auto& entry : m_label_section->m_label_entries) {
    result += entry.second + ": |-\n" + m_text_section->m_text_entries[entry.first].ToText(2) + '\n';
  }

  return result;
}

MSBT FromBinary(tcb::span<const u8> data) {
  return MSBT{data};
}

MSBT FromText(std::string_view text) {  
  return MSBT{text};
}

}  // namespace oepd::msbt