#pragma once
#include "filesystem/localfilesystem.h"
#ifdef _WIN32
#include <Windows.h>
#endif

#include "log/log.h"
namespace WhiteDeer {
namespace Engine {

using std::string;
namespace fs = std::filesystem;
using namespace WhiteDeer::Utils;
class FileSystem;

class FileSystemManager : public Singleton<FileSystemManager>,
                          public Registry<FileSystem> {
  friend class Singleton<FileSystemManager>;

 public:
  static LocalFileSystem* GetLocalFileSystem() {
    return GetInstance()->m_localfs;
  }

 protected:
  FileSystemManager() {
    LOGD.printf("init file system manager");

    fs::path root("");
#ifdef _WIN32
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    root = path;
    while (!root.empty() && root.filename() != "white-deer") {
      root = root.parent_path();
    }
    LOGD.printf("root: %s", root.u8string().c_str());
#endif

    m_localfs = new LocalFileSystem(root);
    Register(m_localfs);
  }

  LocalFileSystem* m_localfs;
};

}  // namespace Engine
}  // namespace WhiteDeer