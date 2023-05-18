#include "msbt/msbt.h"

namespace oepd {

namespace msbt {

namespace tags {

std::wstring GetText(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  return {};
}

tcb::span<const u8> GetBinary(std::wstring tag) {
  return {};
}

}  // namespace tags

}  // namespace msbt

}  // namespace oepd