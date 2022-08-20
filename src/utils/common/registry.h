#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>
#include <string>

using std::map;
using std::set;
using std::shared_ptr;
using std::vector;
using std::string;

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
 public:
  using PTR = shared_ptr<V>;
  using CONTAINER = map<K, PTR>;

 public:
  static CONTAINER& GetEntries() { return s_entries; }

  template <typename... Args>
  static PTR GetOrLoad(const K& name, Args... args) {
    // string newname = name;
    string newname = name.empty() ? NewName() : name;
    if (s_entries.find(newname) == s_entries.end()) {
      s_entries[newname] = PTR(new V(newname, args...));
    }

    return s_entries[newname];
  }

  static PTR Set(const K& name, PTR ptr) {
    // string newname = name;
    string newname = name.empty() ? NewName() : name;
    if (s_entries.find(newname) == s_entries.end()) {
      s_entries[newname] = ptr;
    }
    return ptr;
  }

  static PTR Set(PTR ptr) { return Set("", ptr); }

  static void Delete(const K& name) {
    if (s_entries.find(name) != s_entries.end()) {
      s_entries.erase(name);
    }
  }

  static string NewName() {
    static int s_nameindex = 0;
    return "_anonymous" + std::to_string(s_nameindex++);
  }

 protected:
  static CONTAINER s_entries;
};

}  // namespace Utils
}  // namespace WhiteDeer