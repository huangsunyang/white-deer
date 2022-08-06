#pragma once
#include <vector>

#include "utils/common/registry.h"
#include "utils/common/singleton.h"

namespace WhiteDeer {
namespace Editor {

using std::vector;
using namespace WhiteDeer::Utils;

class EditorGUI;

class EditorGUIManager : public Singleton<EditorGUIManager>, public Registry<EditorGUI> {
public:
  void Render();
  void InitEditors();
};

class EditorGUI {
private:
  friend class EditorGUIManager;

protected:
  EditorGUI() {}
  virtual void Render() {} // inplement render for custom editor guis
  bool m_showing = true;

};

template <typename T>
class EditorRegistry: public Singleton<T> {
public:
  static void Register() {
    EditorGUIManager::GetInstance()->Register(GetInstance());
  }
};

} // namespace Editor
} // namespace WhiteDeer