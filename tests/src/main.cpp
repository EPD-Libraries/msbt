#include <iostream>
#include <msbt/msbt.h>

#include "utils/file_util.h"

int main(int argc, char** argv) {
  std::string path{argv[1]};
  std::string ext = *file::util::get_extension(path);

  if (ext == ".yaml" || ext == ".yml") {
    const auto file = file::util::read_all_bytes(path);
    auto msbt = oepd::msbt::FromText(reinterpret_cast<const char*>(file.data()));

    size_t idx = file::util::get_extension_index(path);
    std::string outfile = path.substr(0, idx) + ".msbt";

    std::ofstream stream(outfile, std::ios::binary);
    stream << msbt.ToBinary().data();
  } else if (ext == ".msbt") {
    const auto file = file::util::read_all_bytes(path);
    auto msbt = oepd::msbt::FromBinary(file);

    size_t idx = file::util::get_extension_index(path);
    std::string outfile = path.substr(0, idx) + ".yml";

    std::ofstream stream(outfile, std::ios::binary);
    stream << msbt.ToText();
  }
}