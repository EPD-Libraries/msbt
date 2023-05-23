#include "msbt/tags.h"
#include "util.h"

namespace oepd::msbt::tags {

Tag* CreateTag(u16 group_id, u16 type_id, tcb::span<const u8> data) {
  Tag* tag;
  if (group_id == 0 && type_id == 2) {
    tag = new FontSizeTag;
  } else {
    tag = new UnknownTag;
  }

  tag->Fill(group_id, type_id, data);
  return tag;
}

Tag* CreateTag(std::string_view text) {
  Tag* tag;
  const auto tag_info = util::parse_tag(text);

  if (tag_info.first == "FontSize") {
    tag = new FontSizeTag;
  } else {
    tag = new UnknownTag;
  }

  tag->Fill(tag_info.first, tag_info.second);
  return tag;
}

}  // namespace oepd::msbt::tags