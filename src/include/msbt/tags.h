#pragma once

#include <exio/types.h>
#include <nonstd/span.h>

namespace oepd::msbt::tags {

/// Converts tag meta-data to a pseudo-HTML UTF-16 string representation
std::wstring GetText(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  // All tag types will be identified here
  // and sent to a tag class for processing
  return {};
}

tcb::span<const u8> GetBinary(std::wstring tag) {
  return {};
}

}  // namespace oepd::msbt::tags