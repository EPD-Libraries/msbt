#pragma once

#include <exio/binary_writer.h>
#include <exio/error.h>
#include <iostream>
#include <optional>
#include <vector>

namespace oepd::msbt::tags {

using TagParams = std::vector<std::pair<std::string, std::string>>;

class Tag {
public:
  /// Fill the tag data from binary meta data
  virtual void Fill(u16 group_id, u16 type_id, tcb::span<const u8> data) = 0;

  /// Fill the tag data from pseudo-HTML text data
  virtual void Fill(std::string_view group_name, TagParams params) = 0;

  virtual std::string ToText() = 0;
  virtual void ToBinary(exio::BinaryWriter& writer) = 0;
};

/// Finds and creates a new tag from the provided arguments
Tag* CreateTag(u16 group_id, u16 type_id, tcb::span<const u8> data);

/// Finds and constructs a tag from the string representation
Tag* CreateTag(std::string_view text);

class UnknownTag : public Tag {
public:
  void Fill(u16 group_id, u16 type_id, tcb::span<const u8> data) override;
  void Fill(std::string_view group_name, TagParams params) override;

  std::string ToText() override;
  void ToBinary(exio::BinaryWriter& writer) override;

  u16 m_group;
  u16 m_type;
  tcb::span<const u8> m_data;

private:
  std::optional<std::vector<u8>> m_private_data;
};

}  // namespace oepd::msbt::tags