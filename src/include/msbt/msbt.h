#pragma once

#include <exio/binary_reader.h>
#include <exio/binary_writer.h>
#include <exio/error.h>
#include <exio/swap.h>
#include <exio/types.h>
#include <exio/util/magic_utils.h>
#include <nonstd/span.h>

namespace oepd {

namespace msbt {

struct LabelSection {
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
  LabelSection(exio::BinaryReader& reader);
  std::vector<std::pair<size_t, std::string>> m_label_entries{};
};

struct AttributeSection {
public:
  AttributeSection(exio::BinaryReader& reader);
};

struct TextSection {
private:
  struct OffsetTable {
    u32 offset_count;
  };
  static_assert(sizeof(OffsetTable) == 0x04);

public:
  TextSection(exio::BinaryReader& reader, size_t eof);
  std::vector<tcb::span<const u8>> m_text_entries;
};

class MSBT {
public:
  MSBT(tcb::span<const u8> data);

  std::vector<u8> ToBinary();

  std::optional<LabelSection> m_label_section;
  std::optional<AttributeSection> m_attribute_section;
  std::optional<TextSection> m_text_section;

private:
  exio::BinaryReader m_reader;
};

MSBT FromBinary(tcb::span<const u8> data);

}  // namespace msbt

}  // namespace oepd