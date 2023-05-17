#include <codecvt>
#include <iostream>
#include <locale>
#include <msbt/msbt.h>
#include <string>

#include "utils/file_util.h"

int main(int argc, char** argv) {
  std::cout << "[c++] Init Testing" << std::endl;
  const auto file = file::util::ReadAllBytes(argv[1]);
  auto msbt = oepd::msbt::FromBinary(file);

  std::ofstream stream(argv[2], std::ios::binary);
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

  for (const auto entry : msbt.m_label_section->m_label_entries) {
    stream << entry.second << ": |" << std::endl
           << converter.to_bytes(msbt.m_text_section->m_text_entries[entry.first].ToText(2)) << std::endl;
  }
}