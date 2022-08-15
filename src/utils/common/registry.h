#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

using std::map;
using std::set;
using std::shared_ptr;
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

// use K to index shared_ptr<V>
template <typename K, typename V>
class StaticNamedPool {
  using RAWPTR = V*;
  using PTR = shared_ptr<V>;
  using CONTAINER = map<K, PTR>;

 public:
  static CONTAINER& GetEntries() { return s_entries; }

  template <typename... Args>
  static PTR GetOrLoad(const K& name, Args... args) {
    if (s_entries.find(name) != s_entries.end()) {
      return s_entries[name];
    }
    RAWPTR rawptr = new V;
    rawptr->load(name, args...);
    PTR ptr(rawptr);
    s_entries[name] = ptr;
    return ptr;
  }

  static void Delete(const K& name) {
    if (s_entries.find(name) != s_entries.end()) {
      s_entries.erase(name);
    }
  }

 protected:
  static CONTAINER s_entries;
};

}  // namespace Utils
}  // namespace WhiteDeer