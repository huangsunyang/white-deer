#pragma once
#define NOMINMAX 1
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
#include "utils/common/registry.h"
#include "utils/common/singleton.h"

using std::shared_ptr;
using std::string, std::map, std::set, std::vector;
using namespace WhiteDeer::Utils;

namespace WhiteDeer {
namespace Graphics {

class Program;

class Shader : public StaticNamedPool<string, Shader> {
  friend class Program;
  friend class StaticNamedPool<string, Shader>;

 public:
  ~Shader();

  GLuint GetHandle() { return m_handle; }
  string GetName() { return m_name; }

  static auto &GetAllShaders() { return GetEntries(); }
  static void ReloadAll();

 protected:
  Shader(const string &s) { load(s); }
  void load(const string &);

  string m_name;
  GLuint m_handle;
  //   set<shared_ptr<Program>> m_programs;
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

  void SetUniformTexture(const string &name, GLuint handle, int index = 0) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, handle);
    glUniform1i(GetUniformLocation(name), index);
  }

  void SetUniformMatrix4fv(const string &name, const glm::mat4 &matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE,
                       glm::value_ptr(matrix));
  }

 public:
  template <typename... Args>
  static shared_ptr<Program> Load(const Args... paths) {
    shared_ptr<Program> p_program(new Program);
    p_program->load({paths...});
    s_programs.insert(p_program);
    return p_program;
  }

  static void Delete(const string &);
  static void RefreshAll();
  static auto &GetAllPrograms() { return s_programs; };

 protected:
  Program() { m_handle = glCreateProgram(); }

  void load(std::initializer_list<string> paths);
  void load(const vector<string> &paths);

  GLuint m_handle;
  set<shared_ptr<Shader>> m_shaders;

  static set<shared_ptr<Program>> s_programs;
};

}  // namespace Graphics
}  // namespace WhiteDeer
