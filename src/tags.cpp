#include "msbt/tags.h"
#include "util.h"


namespace oepd::msbt::tags {
  
#define TagCase(a, b) (a + 1) * (b + 1)

Tag* CreateTag(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  Tag* tag;
  s16 match = group_id - (type_id * type_id);
  switch (match) {
    case TagCase(0, 0):
      tag = new FontTag;
      break;
    case TagCase(0, 2):
      tag = new FontSizeTag;
      break;
    case TagCase(0, 3):
      tag = new FontColorTag;
      break;
    case TagCase(2, 1):
      tag = new StringFlagTag;
      break;
    case TagCase(2, 2):
      tag = new IntegerFlagTag;
      break;
    case TagCase(2, 9):
      tag = new FloatFlagTag;
      break;
    default:
      tag = new UnknownTag;
      break;
  }

  tag->Fill(group_id, type_id, data);
  return tag;
}

Tag* CreateTag(std::string_view text) {
  Tag* tag;
  const auto tag_info = util::parse_tag(text);

  if (tag_info.first == "Font") {
    tag = new FontTag;
  } else if (tag_info.first == "FontSize") {
    tag = new FontSizeTag;
  } else if (tag_info.first == "FontColor") {
    tag = new FontColorTag;
  } else if (tag_info.first == "IntFlag") {
    tag = new IntegerFlagTag;
  } else if (tag_info.first == "StringFlag") {
    tag = new StringFlagTag;
  } else if (tag_info.first == "FloatFlag") {
    tag = new FloatFlagTag;
  } else {
    tag = new UnknownTag;
  }

  tag->Fill(tag_info.first, tag_info.second);
  return tag;
}

}  // namespace oepd::msbt::tags