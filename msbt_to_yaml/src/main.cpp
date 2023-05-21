#include <iostream>
#include <msbt/msbt.h>

#include "io/file.h"

int main(int argc, char** argv) {
  std::string path{argv[1]};
  std::string ext = *io::file::get_extension(path);

  if (ext == ".yaml" || ext == ".yml") {
    const auto file = io::file::read_all_bytes(path);
    auto msbt = oepd::msbt::FromText(std::string{reinterpret_cast<const char*>(file.data()), file.size()});
    const auto data = msbt.ToBinary();

    size_t idx = io::file::get_extension_index(path);
    std::string outfile = path.substr(0, idx) + ".msbt";

    std::ofstream stream(outfile, std::ios::binary);
    stream.write(reinterpret_cast<const char*>(data.data()), data.size());
  } else if (ext == ".msbt") {
    const auto file = io::file::read_all_bytes(path);
    auto msbt = oepd::msbt::FromBinary(file);

    size_t idx = io::file::get_extension_index(path);
    std::string outfile = path.substr(0, idx) + ".yml";

    std::ofstream stream(outfile, std::ios::binary);
    stream << msbt.ToText();
  }
}