#include "msbt/msbt.h"

namespace oepd::msbt {

LabelSection::LabelSection(exio::BinaryReader& reader) {
  const auto label_table_offset = reader.Tell();
  const auto table = *reader.Read<SectionTable>();

  for (size_t i = 0; i < table.offset_count; i++) {
    const auto offset_entry = *reader.Read<LabelEntry>(label_table_offset + sizeof(SectionTable) + sizeof(LabelEntry) * i);
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

}  // namespace oepd::msbt
