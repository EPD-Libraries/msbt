#include <iostream>
#include <msbt/msbt.h>

#include "utils/file_util.h"

int main(int argc, char** argv) {
  std::cout << "[c++] Init Testing" << std::endl;
  const auto file = file::util::ReadAllBytes(argv[1]);
  auto msbt = oepd::msbt::FromBinary(file);

  for (const auto entry : msbt.m_label_section->m_label_entries) {
    std::cout << entry.second << std::endl;
  }

  std::cout << "Count: " << msbt.m_label_section->m_label_entries.size() << std::endl;

  std::ofstream stream(argv[2], std::ios::binary);
  const auto data = msbt.ToBinary();
  stream.write(reinterpret_cast<const char*>(data.data()), data.size());
}