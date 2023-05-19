#pragma once

#include <exio/binary_reader.h>
#include <exio/binary_writer.h>
#include <exio/error.h>
#include <exio/swap.h>
#include <exio/types.h>
#include <exio/util/magic_utils.h>
#include <msbt/tags.h>
#include <nonstd/span.h>

namespace oepd::msbt {

/// Common Section Header
struct SectionHeader {
  std::array<char, 4> magic;
  u32 table_size;
  u64 _padding;
};
static_assert(sizeof(SectionHeader) == 0x10);

/// Common Section Table
struct SectionTable {
  u32 offset_count;
};
static_assert(sizeof(SectionTable) == 0x04);

/// MSBT Label Section (LBL1)
struct LabelSection {
  struct LabelEntry {
    u32 string_count;
    u32 string_offset;
  };
  static_assert(sizeof(LabelEntry) == 0x08);

public:
  LabelSection(exio::BinaryReader& reader);
  std::vector<std::pair<size_t, std::string>> m_label_entries{};
};

/// MSBT Attribute Section (ATR1)
struct AttributeSection {
public:
  AttributeSection(exio::BinaryReader& reader);
};

/// MSBT Text Section (TXT2)
struct TextSection {
public:
  struct TextEntryValue {
  public:
    TextEntryValue(tags::Tag* tag);
    TextEntryValue(std::wstring text);

    tags::Tag* m_tag = nullptr;
    std::optional<std::wstring> m_text;
  };

  struct TextEntry {
    void Fill(tcb::span<const u8> data);

    static TextEntry FromText(std::string text);
    std::string ToText(size_t indent_level = 0, bool one_line = false);
    std::vector<TextEntryValue> m_values;
  };

  TextSection(exio::BinaryReader& reader, size_t table_size);
  std::vector<TextEntry> m_text_entries;
};

class MSBT {
public:
  MSBT(tcb::span<const u8> data);
  MSBT(std::string text);

  std::vector<u8> ToBinary();
  std::string ToText();

  std::optional<LabelSection> m_label_section;
  std::optional<AttributeSection> m_attribute_section;
  std::optional<TextSection> m_text_section;

private:
  exio::BinaryReader m_reader;
};

MSBT FromBinary(tcb::span<const u8> data);
MSBT FromText(std::string text);

}  // namespace oepd::msbt