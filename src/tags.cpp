#include "msbt/tags.h"
#include "util.h"

namespace oepd::msbt::tags {

Tag* FillTag(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  Tag* tag;
  if (group_id == 201) {
    tag = new UnknownTag;
  } else {
    tag = new UnknownTag;
  }

  tag->Fill(group_id, type_id, data);
  return tag;
}

void UnknownTag::Fill(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  m_group = group_id;
  m_type = type_id;
  m_data = data;
}

void UnknownTag::Fill(std::string text) {
  if (text[0] != '<' || text.back() != '>') {
    throw exio::InvalidDataError("Invalid tag syntax");
  }

  size_t pos = 0;

  // Parse group name
  std::string group;
  while (text[++pos] != ' ') {
    group += text[pos];
  }

  m_group = stoi(group);

  // Parse node params
  const auto params = util::parse_params(text, --pos);
  for (const auto entry : params) {
    if (entry.first == "Type") {
      m_type = stoi(entry.second);
    } else if (entry.first == "Data") {
      m_private_data = util::from_hex(entry.second);
      m_data = *m_private_data;
    }
  }
}

std::string UnknownTag::ToText() {
  return "<" + std::to_string(m_group) + " Type='" + std::to_string(m_type) +
         (m_data.size() > 0 ? "' Data='" + util::to_hex(m_data) + "'/>" : "'/>");
}

void UnknownTag::ToBinary(exio::BinaryWriter& writer) {}

}  // namespace oepd::msbt::tags