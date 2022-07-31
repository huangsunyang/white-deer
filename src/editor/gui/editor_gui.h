#pragma once
#include <vector>

#include "utils/common/singleton.h"

namespace WhiteDeer {
namespace Editor {

using std::vector;
using namespace WhiteDeer::Utils;

class EditorGUI;

class EditorGUIManager : public Singleton<EditorGUIManager> {
public:
  void Render();
  void InitEditors();
  bool HasEditor(EditorGUI *);
  void RegisterEditor(EditorGUI *);
  void UnRegisterEditor(EditorGUI *);

protected:
  vector<EditorGUI *> m_editorGUIVec;
};

class EditorGUI : public Singleton<EditorGUI> {
private:
  friend class EditorGUIManager;

protected:
  virtual void Render() {} // inplement render for custom editor guis
  bool m_showing = true;

};

template <typename T>
class EditorRegistry: public Singleton<T> {
public:
  static void Register() {
    EditorGUIManager::GetInstance().RegisterEditor(&GetInstance());
  }
};

} // namespace Editor
} // namespace WhiteDeer