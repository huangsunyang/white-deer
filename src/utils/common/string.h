#pragma once
#include <algorithm>
#include <string>

namespace WhiteDeer {
namespace Utils {
inline bool endswith(const std::string& str, const std::string& ending) {
  if (ending.size() > str.size()) return false;
  return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
}
}  // namespace Utils
}  // namespace WhiteDeer