#pragma once

#include <filesystem>

#include "filesystem/filesystem.h"

namespace WhiteDeer {
namespace Engine {

namespace fs = std::filesystem;

class LocalFileSystem : public FileSystem {
 public:
  LocalFileSystem(fs::path p) : FileSystem(p) {}

  virtual bool CanLocate(fs::path path) override;
  virtual fs::path ToAbsolute(fs::path path) override;

  virtual bool Open(fs::path path) override { return true; }
};

LocalFileSystem* GetLocalFileSystem();
}  // namespace Engine
}  // namespace WhiteDeer