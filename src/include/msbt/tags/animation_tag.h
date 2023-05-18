#pragma once

#include "msbt/tags.h"

namespace oepd::msbt::tags {

constexpr std::string_view name = "Animation";

class AnimationTag : public Tag {
public:
  std::string Deserialize();
};

}  // namespace oepd::msbt::tags