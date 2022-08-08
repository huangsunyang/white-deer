#pragma once
#include <GL/gl3w.h>

#include <map>
#include <memory>
#include <set>
#include <string>

#include "utils/common/singleton.h"

using std::shared_ptr;
using std::string, std::map, std::set;
using namespace WhiteDeer::Utils;

namespace WhiteDeer {
namespace Graphics {

class Program;

class Shader {
  friend class Program;

 public:
  Shader(const string &);
  ~Shader();

  static shared_ptr<Shader> Load(const string &, bool = false);
  static void Delete(const string &);
  static void ReloadAll();

 protected:
  string m_name;
  GLuint m_handle;
  //   set<shared_ptr<Program>> m_programs;

  static map<string, shared_ptr<Shader>> m_shaders;
  //   static map<GLuint, shared_ptr<Shader>> m_shaders;
};

class Program {
 public:
  Program(const string&, const string&);
  ~Program();

  void Refresh();
  void Use() { glUseProgram(m_handle); }

  static shared_ptr<Program> Load(const string &, const string &);
  static void Delete(const string &);
  static void RefreshAll();

 protected:
  void _Load(const string&, const string&);

  GLuint m_handle;
  set<shared_ptr<Shader>> m_shaders;

  static set<shared_ptr<Program>> m_programs;
};

}  // namespace Graphics
}  // namespace WhiteDeer