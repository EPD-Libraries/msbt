#pragma once

#include <exio/types.h>
#include <iostream>
#include <nonstd/span.h>
#include <vector>

namespace oepd::msbt::tags {

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

std::string hex_str(tcb::span<const u8> data);

class Tag {
public:
  /// Fill the tag data from binary meta data
  virtual void Fill(u16 group_id, u16 type_id, tcb::span<const u8> data) = 0;

  /// Fill the tag data from pseudo-HTML text data
  virtual void Fill(std::string text) = 0;

  virtual std::string ToText() = 0;
  virtual std::vector<u8> ToBinary() = 0;
};

/// Finds and create a new tag based from the provided arguments
Tag* FillTag(u16 group_id, u16 type_id, tcb::span<const u8> data);

class UnknownTag : public Tag {
public:
  void Fill(u16 group_id, u16 type_id, tcb::span<const u8> data) override;
  void Fill(std::string text) override;

  std::string ToText() override;
  std::vector<u8> ToBinary() override;

  u16 m_group;
  u16 m_type;
  tcb::span<const u8> m_data;
};

}  // namespace oepd::msbt::tags