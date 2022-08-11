#include "filesystem/filesystemmanager.h"
namespace WhiteDeer {
namespace Engine {
LocalFileSystem* GetLocalFileSystem() {
  return FileSystemManager::GetInstance()->GetLocalFileSystem();
}

bool LocalFileSystem::CanLocate(fs::path path) {
  return fs::exists(ToAbsolute(path));
}

fs::path LocalFileSystem::ToAbsolute(fs::path path) {
  if (!path.is_absolute()) {
    return m_root / path;
  }
  return path;
}
}  // namespace Engine
}  // namespace WhiteDeer