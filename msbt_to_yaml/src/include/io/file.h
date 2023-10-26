#pragma once

#include <exio/types.h>
#include <fstream>
#include <iostream>
#include <nonstd/span.h>
#include <vector>

namespace io::file {

std::vector<u8> read_all_bytes(std::string& file_path) {
  std::vector<u8> file;
  std::ifstream stream{file_path, std::ios::binary | std::ios::ate};
  file.resize(stream.tellg());
  stream.seekg(0, std::ios::beg);
  stream.read(reinterpret_cast<char*>(file.data()), file.size());
  return file;
}

size_t get_extension_index(std::string& path) {
  size_t pos = 0;
  size_t index = 0;
  while ((pos = path.find('.', pos)) != std::string::npos) {
    index = pos++;
  }

  return index;
}

std::optional<std::string> get_extension(std::string& path) {
  return path.substr(get_extension_index(path), path.length());
}

}  // namespace io::file