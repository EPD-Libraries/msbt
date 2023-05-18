#include "msbt/tags.h"

namespace oepd::msbt::tags {

std::wstring GetText(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  // All tag types will be identified here
  // and sent to a tag class for processing
  return {};
}

tcb::span<const u8> GetBinary(std::wstring tag) {
  return {};
}

}  // namespace oepd::msbt::tags