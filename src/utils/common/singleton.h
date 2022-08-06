#pragma once

namespace WhiteDeer {
namespace Utils {

template <typename T>
class Singleton {
 public:
  static T *GetInstance();

  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;

 protected:
  Singleton(){};
};

template <typename T>
T *Singleton<T>::GetInstance() {
  static T s_instance;
  return &s_instance;
}

}  // namespace Utils
}  // namespace WhiteDeer