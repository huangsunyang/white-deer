#include "filesystem/filesystemmanager.h"
namespace WhiteDeer {
namespace Engine {
LocalFileSystem* GetLocalFileSystem() {
  return FileSystemManager::GetInstance()->GetLocalFileSystem();
}
}  // namespace Engine
}  // namespace WhiteDeer