#include <iostream>
#include <restbl/restbl.h>

#include "utils/file_util.h"

int main(int argc, char** argv) {
  std::cout << "[c++] Init Testing" << std::endl;
  const auto file = file::util::ReadAllBytes(argv[1]);
  oepd::restbl::RESTBL restbl{file};

  std::ofstream stream(argv[2], std::ios::binary);
  const auto data = restbl.ToBinary();
  stream.write(reinterpret_cast<const char*>(data.data()), data.size());
}