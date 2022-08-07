#pragma once

#include <filesystem>

#include "filesystem/filesystem.h"

namespace WhiteDeer {
namespace Engine {

namespace fs = std::filesystem;

class LocalFileSystem : public FileSystem {
 public:
  LocalFileSystem(fs::path p): FileSystem(p) {}

  virtual bool CanLocate(fs::path path) override {
    return fs::exists(ToAbsolute(path));
  }

  virtual fs::path ToAbsolute(fs::path path) override {
    if (!path.is_absolute()) {
      return m_root / path;
    }
    return path;
  }

  virtual bool Open(fs::path path) override { return true; }
};

LocalFileSystem* GetLocalFileSystem();
}  // namespace Engine
}  // namespace WhiteDeer