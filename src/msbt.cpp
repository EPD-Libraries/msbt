#include "msbt/msbt.h"

namespace oepd {

namespace msbt {

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

struct TableHeader {
  std::array<char, 4> magic;
  u32 table_size;
  u64 _padding;
};
static_assert(sizeof(TableHeader) == 0x10);

struct LabelSection {
private:
  struct OffsetTable {
    u32 offset_count;
  };
  static_assert(sizeof(OffsetTable) == 0x04);

  struct OffsetTableEntry {
    u32 string_count;
    u32 string_offset;
  };
  static_assert(sizeof(OffsetTableEntry) == 0x08);

public:
  LabelSection(exio::BinaryReader& reader) {
    const auto label_table_offset = reader.Tell();
    const auto table = *reader.Read<OffsetTable>();

    for (size_t i = 0; i < table.offset_count; i++) {
      const auto offset_entry =
          *reader.Read<OffsetTableEntry>(label_table_offset + sizeof(OffsetTable) + sizeof(OffsetTableEntry) * i);

      size_t offset = label_table_offset + offset_entry.string_offset;
      for (size_t j = 0; j < offset_entry.string_count; j++) {
        const auto size = *reader.Read<u8>(offset);
        const auto label = reader.ReadString(offset += sizeof(u8), size);
        const auto index = *reader.Read<u32>(offset += size);
        offset += sizeof(u32);

        m_label_entries.push_back(std::pair<size_t, std::string>{index, label});
      }
    }
  }

  std::vector<std::pair<size_t, std::string>> m_label_entries{};
};

struct AttributeSection {
public:
  AttributeSection(exio::BinaryReader& reader) {}
};

struct TextSection {
private:
  struct OffsetTable {
    u32 offset_count;
  };
  static_assert(sizeof(OffsetTable) == 0x04);

public:
  TextSection(exio::BinaryReader& reader, size_t eof) {
    const auto text_table_offset = reader.Tell();
    const auto table = *reader.Read<OffsetTable>();
    m_text_entries = std::vector<std::wstring>{table.offset_count};

    for (size_t i = 0; i < table.offset_count; i++) {
      const auto offset_ptr = text_table_offset + sizeof(OffsetTable) + sizeof(u32) * i;
      const auto offset = *reader.Read<u32>(offset_ptr);
      auto next_offset = *reader.Read<u32>(offset_ptr + sizeof(u32));

      // Don't read past EOF
      if (i == table.offset_count - 1) {
        next_offset = eof - text_table_offset;
      }

      m_text_entries[i] = reader.ReadWString(text_table_offset + offset, next_offset - offset);
    }

    reader.Seek(eof);
  }

  std::vector<std::wstring> m_text_entries;
};

MSBT::MSBT(tcb::span<const u8> data) : m_reader{data, exio::Endianness::Little} {
  const auto header = *m_reader.Read<Header>();

  if (header.magic != MsbtMagic) {
    throw exio::InvalidDataError("Invalid MSBT magic");
  }

  std::optional<LabelSection> label_section;
  std::optional<AttributeSection> attribute_section;
  std::optional<TextSection> text_section;

  for (size_t i = 0; i < header.num_sections; i++) {
    const auto magic = m_reader.Read<TableHeader>()->magic;
    if (magic == LabelSectionMagic) {
      label_section = LabelSection{m_reader};
    } else if (magic == AttributeSectionMagic) {
      attribute_section = AttributeSection{m_reader};
    } else if (magic == TextSectionMagic) {
      text_section = TextSection{m_reader, header.file_size};
    } else {
      throw exio::InvalidDataError("Unsupported data block: " + std::string{magic.begin(), magic.end()});
    }

    m_reader.Seek(exio::util::AlignUp(m_reader.Tell(), 0x10));
  }

  if (!label_section) {
    throw exio::InvalidDataError("The label section (LBL1) was not found");
  } else if (!text_section) {
    throw exio::InvalidDataError("The text section (TXT2) was not found");
  }
}

std::vector<u8> MSBT::ToBinary() {
  return {};
}

MSBT FromBinary(tcb::span<const u8> data) {
  return MSBT{data};
}

}  // namespace msbt

}  // namespace oepd