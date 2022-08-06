#pragma once

#include <algorithm>
#include <filesystem>
#include <string>

#include "utils/common/registry.h"
#include "utils/common/singleton.h"

namespace WhiteDeer {
namespace Engine {

using std::string;
namespace fs = std::filesystem;

class FileSystem {
 public:
  FileSystem(fs::path root) : m_root(root) {}

  virtual bool CanLocate(fs::path) = 0;
  virtual fs::path ToAbsolute(fs::path) = 0;
  virtual bool Open(fs::path) = 0;

 protected:
  fs::path m_root;
};

}  // namespace Engine
}  // namespace WhiteDeer