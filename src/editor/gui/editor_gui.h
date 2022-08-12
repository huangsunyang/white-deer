#pragma once
#include <vector>

#include "utils/common/registry.h"
#include "utils/common/singleton.h"

using std::vector;
using WhiteDeer::Utils::Singleton;
using WhiteDeer::Utils::Registry;

namespace WhiteDeer {
namespace Editor {

class EditorGUI;

class EditorGUIManager : public Singleton<EditorGUIManager>,
                         public Registry<EditorGUI> {
 public:
  void Render();
  void InitEditors();
};

class EditorGUI {
 private:
  friend class EditorGUIManager;

 protected:
  EditorGUI() {}
  virtual void Render() {}  // inplement render for custom editor guis
  bool m_showing = true;
};

template <typename T>
class EditorRegistry : public Singleton<T> {
 public:
  static void Register() {
    EditorGUIManager::GetInstance()->Register(GetInstance());
  }
};

}  // namespace Editor
}  // namespace WhiteDeer