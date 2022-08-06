#pragma once

#include <set>
#include <vector>

using std::set;
using std::vector;

namespace WhiteDeer {
namespace Utils {
template <typename T>
class Registry {
 public:
  void Register(T* entry) { m_entries.insert(entry); }
  void UnRegister(T* entry) {
    m_entries.erase(entry);
    delete entry;
  }
  bool IsRegistered(T* entry) { m_entries.find(entry) != m_entries.end(); }

 protected:
  set<T*> m_entries;
};
}  // namespace Utils
}  // namespace WhiteDeer