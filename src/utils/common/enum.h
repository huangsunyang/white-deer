#pragma once
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;

template <typename T>
inline vector<string> GetEnumItems() {
  retrun vector<string>();
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
