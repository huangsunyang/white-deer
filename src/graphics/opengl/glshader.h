#pragma once
#include <GL/gl3w.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "graphics/opengl/glTexture.h"
#include "graphics/opengl/glerror.h"
#include "utils/common/singleton.h"

using std::shared_ptr;
using std::string, std::map, std::set, std::vector;
using namespace WhiteDeer::Utils;

namespace WhiteDeer {
namespace Graphics {

class Program;

class Shader {
  friend class Program;

 public:
  Shader(const string &);
  ~Shader();
  GLuint GetHandle() { return m_handle; }
  string GetName() { return m_name; }

  static shared_ptr<Shader> Load(const string &, bool = false);
  static auto &GetAllShaders() { return s_shaders; }
  static void Delete(const string &);
  static void ReloadAll();

 protected:
  string m_name;
  GLuint m_handle;
  //   set<shared_ptr<Program>> m_programs;

  static map<string, shared_ptr<Shader>> s_shaders;
  //   static map<GLuint, shared_ptr<Shader>> m_shaders;
};

class Program {
 public:
  ~Program();

  void Refresh();
  void Use() { glUseProgram(m_handle); }
  GLuint GetHandle() { return m_handle; }
  auto &GetAllShaders() { return m_shaders; }

  int GetUniformLocation(const string &name) {
    auto result = glGetUniformLocation(m_handle, name.c_str());
    if (result < 0) {
      LOGE << name << ": " << result;
    }
    return result;
  }

  void SetUniform4f(const string &name, float x, float y, float z, float w) {
    glUniform4f(GetUniformLocation(name), x, y, z, w);
  }

  void SetUniformTexture(const string &name, const Texture &tex,
                         int index = 0) {
    tex.Use(index);
    glUniform1i(GetUniformLocation(name), index);
  }

  void SetUniformMatrix4fv(const string &name, const glm::mat4 &matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE,
                       glm::value_ptr(matrix));
  }

 public:
  template <typename... Args>
  static shared_ptr<Program> Load(const Args... paths) {
    shared_ptr<Program> p_program(new Program(paths...));
    s_programs.insert(p_program);
    return p_program;
  }

  static void Delete(const string &);
  static void RefreshAll();
  static auto &GetAllPrograms() { return s_programs; };

 protected:
  Program() { m_handle = glCreateProgram(); }

  template <typename... Args>
  Program(Args &&...path) : Program() {
    _Load({std::forward<Args>(path)...});
  }

  void _Load(std::initializer_list<string> paths);
  void _Load(const vector<string> &paths);

  GLuint m_handle;
  set<shared_ptr<Shader>> m_shaders;

  static set<shared_ptr<Program>> s_programs;
};

}  // namespace Graphics
}  // namespace WhiteDeer