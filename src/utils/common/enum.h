#pragma once
#include <sstream>
#include <string>
#include <vector>

#include "log/log.h"

using std::string;
using std::stringstream;
using std::vector;

template <typename T>
inline vector<string> GetEnumItems() {
  retrun vector<string>();
}

template <typename T>
inline string EnumToString(T e) {
  int index = (int)e;
  auto items = GetEnumItems<T>();
  if (index < 0 || index >= items.size()) {
    LOGE << "enum to string out of index";
  }
  return items[index];
}

#define DECALRE_ENUM(name, ...)                \
  enum name { __VA_ARGS__, name##_COUNT };     \
  template <>                                  \
  inline vector<string> GetEnumItems<name>() { \
    vector<string> items;                      \
    string temp;                               \
    stringstream stream(#__VA_ARGS__);         \
    while (stream >> temp) {                   \
      while (temp.back() == ',') {             \
        temp.pop_back();                       \
      }                                        \
      items.push_back(temp);                   \
    }                                          \
    return items;                              \
  }
