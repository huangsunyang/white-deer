#pragma once
#include <vector>

#include "utils/common/registry.h"
#include "utils/common/singleton.h"

using std::vector;
using WhiteDeer::Utils::Registry;
using WhiteDeer::Utils::Singleton;

namespace WhiteDeer {
namespace Editor {

class EditorGUI {
 private:
  friend class EditorGUIManager;

 public:
  virtual void Open() { m_showing = true; }

 protected:
  virtual void Render() {}  // inplement render for custom editor guis

  bool m_showing = true;
};

class EditorGUIManager : public Singleton<EditorGUIManager>,
                         public Registry<EditorGUI> {
 public:
  void Render();
  void InitEditors();
};

template <typename T>
class EditorWindow : public EditorGUI, public Singleton<T> {
 public:
  static void Register() {
    EditorGUIManager::GetInstance()->Register(GetInstance());
  }
};

}  // namespace Editor
}  // namespace WhiteDeer