#include <map>

#include "msbt/msbt.h"

namespace oepd::msbt {

u32 calc_hash(std::string_view label, size_t num_groups) {
  u32 hash;
  for (const char _char : label) {
    hash = hash * 0x492 + _char;
  }

  return (hash & 0xFFFFFFFF) % num_groups;
}

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

      m_label_entries.push_back({index, label});
    }

    reader.Seek(offset);
  }
}

void LabelSection::Write(exio::BinaryWriter& writer, const size_t num_groups) {
  std::map<u32, std::pair<size_t, std::vector<size_t>>> hash_table;
  for (size_t i = 0; i < m_label_entries.size(); i++) {
    u32 hash = calc_hash(m_label_entries[i].second, num_groups);
    hash_table[hash].second.push_back(i);
  }

  writer.Seek(writer.Tell() + sizeof(SectionHeader));
  size_t block_offset = writer.Tell();

  SectionTable table;
  table.offset_count = hash_table.size();
  writer.Write(table);

  size_t label_meta_offset = writer.Tell();
  writer.Seek(writer.Tell() + sizeof(LabelEntry) * hash_table.size());
  for (const auto group : hash_table) {
    hash_table[group.first].first = writer.Tell() - block_offset;
    for (const auto label_index : group.second.second) {
      const auto label = m_label_entries[label_index];
      writer.Write<u8>(label.second.length());
      writer.Write(label.second);
      writer.Write<u32>(label.first);
    }
  }

  const size_t table_size = writer.Tell() - block_offset;

  writer.Seek(label_meta_offset);
  for (auto group : hash_table) {
    writer.Write<u32>(group.second.second.size());
    writer.Write<u32>(group.second.first);
  }

  SectionHeader header;
  header.magic = LabelSectionMagic;
  header.table_size = table_size;
  header._padding = 0;

  writer.Seek(block_offset - sizeof(SectionHeader));
  writer.Write(header);

  writer.Seek(block_offset + table_size);
}

}  // namespace oepd::msbt
