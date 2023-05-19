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

  std::ofstream stream(std::string{argv[1]} + ".yml", std::ios::binary);
  stream << msbt.ToText();
}