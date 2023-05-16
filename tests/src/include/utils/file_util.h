#pragma once

#include <exio/types.h>
#include <fstream>
#include <iostream>
#include <nonstd/span.h>
#include <vector>

namespace file::util {

std::vector<u8> ReadAllBytes(const char* file_path) {
  std::vector<u8> file;
  std::ifstream stream{file_path, std::ios::binary | std::ios::ate};
  file.resize(stream.tellg());
  stream.seekg(0, std::ios::beg);
  stream.read(reinterpret_cast<char*>(file.data()), file.size());
  return std::move(file);
}

}  // namespace file::util