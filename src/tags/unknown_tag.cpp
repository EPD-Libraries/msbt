#include "msbt/tags.h"
#include "util.h"

namespace oepd::msbt::tags {

void UnknownTag::Fill(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  m_group = group_id;
  m_type = type_id;
  m_data = data;
}

void UnknownTag::Fill(std::string_view group_name, TagParams params) {
  std::from_chars(group_name.data(), group_name.data() + group_name.size(), m_group);
  for (const auto& entry : params) {
    if (entry.first == "Type") {
      m_type = stoi(entry.second);
    } else if (entry.first == "Data") {
      m_private_data = util::from_hex(entry.second);
      m_data = *m_private_data;
      std::cout << util::to_hex(m_data) << std::endl;
    }
  }
}

std::string UnknownTag::ToText() {
  return "<" + std::to_string(m_group) + " Type='" + std::to_string(m_type) +
         (m_data.size() > 0 ? "' Data='" + util::to_hex(m_data) + "'/>" : "'/>");
}

void UnknownTag::ToBinary(exio::BinaryWriter& writer) {
  writer.Write<u16>(m_group);
  writer.Write<u16>(m_type);
  writer.Write<u16>(m_data.size());
  writer.WriteBytes(m_data);
}

}  // namespace oepd::msbt::tags
