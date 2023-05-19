#pragma once

#include <exio/binary_writer.h>
#include <exio/error.h>
#include <iostream>
#include <optional>
#include <vector>

namespace oepd::msbt::tags {

class Tag {
public:
  /// Fill the tag data from binary meta data
  virtual void Fill(u16 group_id, u16 type_id, tcb::span<const u8> data) = 0;

  /// Fill the tag data from pseudo-HTML text data
  virtual void Fill(std::string text) = 0;

  virtual std::string ToText() = 0;
  virtual void ToBinary(exio::BinaryWriter& writer) = 0;
};

/// Finds and create a new tag based from the provided arguments
Tag* FillTag(u16 group_id, u16 type_id, tcb::span<const u8> data);

class UnknownTag : public Tag {
public:
  void Fill(u16 group_id, u16 type_id, tcb::span<const u8> data) override;
  void Fill(std::string text) override;

  std::string ToText() override;
  void ToBinary(exio::BinaryWriter& writer) override;

  u16 m_group;
  u16 m_type;
  tcb::span<const u8> m_data;

private:
  std::optional<std::vector<u8>> m_private_data;
};

}  // namespace oepd::msbt::tags