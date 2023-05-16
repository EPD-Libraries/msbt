#pragma once

#include <exio/binary_reader.h>
#include <exio/binary_writer.h>
#include <exio/error.h>
#include <exio/swap.h>
#include <exio/types.h>
#include <exio/util/magic_utils.h>
#include <nonstd/span.h>

namespace oepd {

namespace msbt {

class MSBT {
public:
  MSBT(tcb::span<const u8> data);

  std::vector<u8> ToBinary();

private:
  exio::BinaryReader m_reader;
};

MSBT FromBinary(tcb::span<const u8> data);

}  // namespace msbt

}  // namespace oepd