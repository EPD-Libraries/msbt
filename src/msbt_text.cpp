#include "msbt/msbt.h"
#include "msbt/tags.h"

namespace oepd::msbt {

std::string TextSection::TextEntry::ToText(size_t indent_level, bool one_line) {
  size_t size = m_data.size() / sizeof(wchar_t);
  size_t idx = 0;
  std::wstring wdata{reinterpret_cast<const wchar_t*>(m_data.begin()), size};

  std::string text(indent_level, L' ');

ReadByte:
  if (idx == size) {
    return text;
  }

  if (wdata[idx] == 0x0E) {
    const auto group = *m_reader.Read<u16>(++idx * 2);
    const auto type = *m_reader.Read<u16>(++idx * 2);
    const auto data_size = *m_reader.Read<u16>(++idx * 2);

    text += tags::GetText(group, type, m_data.subspan(++idx * 2, data_size));
    idx += (data_size / 2);
  } else if (wdata[idx] == 0x00) {
    idx++;
  } else if (wdata[idx] == 0x0A) {
    if (one_line) {
      text += "\\n";
    } else {
      text += "\n" + std::string(indent_level, ' ');
    }
    idx++;
  } else {
    text += wdata[idx++];
  }

  goto ReadByte;
}

TextSection::TextEntry TextSection::TextEntry::FromText(std::wstring text) {}

}  // namespace oepd::msbt