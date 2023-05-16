#include "msbt/msbt.h"

namespace oepd {

namespace msbt {

MSBT::MSBT(tcb::span<const u8> data) {}

std::vector<u8> MSBT::ToBinary() {
  return {};
}

MSBT FromBinary(tcb::span<const u8> data) {
  return MSBT{data};
}

}  // namespace msbt

}  // namespace oepd