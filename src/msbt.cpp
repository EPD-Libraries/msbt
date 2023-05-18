#include <codecvt>
#include <locale>
#include <string>

#include "msbt/msbt.h"

namespace oepd::msbt {

constexpr auto MsbtMagic = exio::util::MakeMagic("MsgStdBn");
constexpr auto LabelSectionMagic = exio::util::MakeMagic("LBL1");
constexpr auto AttributeSectionMagic = exio::util::MakeMagic("ATR1");
constexpr auto TextSectionMagic = exio::util::MakeMagic("TXT2");

struct Header {
  std::array<char, 8> magic;
  u16 bom;
  u16 _padding_1;
  u16 version;
  u16 num_sections;
  u16 _padding_2;
  u32 file_size;
  std::array<char, 10> _padding_3;
} __attribute__((packed));
static_assert(sizeof(Header) == 0x20);

MSBT::MSBT(tcb::span<const u8> data) : m_reader{data, exio::Endianness::Little} {
  const auto header = *m_reader.Read<Header>();

  if (header.magic != MsbtMagic) {
    throw exio::InvalidDataError("Invalid MSBT magic");
  }

  for (size_t i = 0; i < header.num_sections; i++) {
    const auto table_header = *m_reader.Read<SectionHeader>();
    const auto magic = table_header.magic;
    if (magic == LabelSectionMagic) {
      m_label_section = LabelSection{m_reader};
    } else if (magic == AttributeSectionMagic) {
      m_attribute_section = AttributeSection{m_reader};
    } else if (magic == TextSectionMagic) {
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

MSBT::MSBT(std::string text) {}

std::vector<u8> MSBT::ToBinary() {
  return {};
}

std::string MSBT::ToText() {
  std::string result;
  for (const auto entry : m_label_section->m_label_entries) {
    result += entry.second + ": |\n" + m_text_section->m_text_entries[entry.first].ToText(2) + '\n';
  }

  return result;
}

MSBT FromBinary(tcb::span<const u8> data) {
  return MSBT{data};
}

MSBT FromText(std::string text) {
  return MSBT{text};
}

}  // namespace oepd::msbt