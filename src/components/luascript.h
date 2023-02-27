#pragma once

#include "components/script.h"
#include "filesystem/localfilesystem.h"
#include "scriptsystem/lua/luamanager.h"

namespace WhiteDeer {
namespace Engine {

class LuaScript : public Script {
  DECLARE_WHITEDEER_TYPE(LuaScript)

 public:
  virtual void Awake() override;
  virtual void Update() override;

  virtual void SetScript(const string& script) { filename = script; }

  template <typename T>
  void Transfer(T* transfer, const string& name) {
    Script::VirtualTransfer(transfer, name);
    transfer->Transfer("filename", &filename);
  }

  string GetFileName() { return filename; }
  void SetFileName(const string& s) { filename = s; }

 protected:
  string filename;
  sol::table m_instance;
};

}  // namespace Engine
}  // namespace WhiteDeer