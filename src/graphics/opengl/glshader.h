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
#include "utils/common/macro.h"
#include "utils/common/registry.h"
#include "utils/common/singleton.h"

using std::shared_ptr;
using std::string, std::map, std::set, std::vector;
using namespace WhiteDeer::Utils;

#define CAT(L, R) L##R
#define FOREACH(x) CAT(REMOVE_, x)

#define REMOVE_int
#define REMOVE_uint
#define REMOVE_float

#define DECLARE_SET_UNIFORM(postfix, ...)                                \
  void SetUniform##postfix(const string &name, __VA_ARGS__) {            \
    if (!HasUniform(name)) return;                                       \
    glUniform##postfix(GetUniformLocation(name), FORMACRO(__VA_ARGS__)); \
  }

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

  Shader(const string &s, int t) { load(s, t); }

 protected:
  Shader(const string &s) { load(s); }
  void load(const string &);
  void load(const string &, int);

  string m_name;
  GLuint m_handle;
};

class Program {
 public:
  ~Program();

  void Refresh();
  void Use() { glUseProgram(m_handle); }
  GLuint GetHandle() { return m_handle; }
  auto &GetAllShaders() { return m_shaders; }

  bool HasUniform(const string &name);
  int GetUniformLocation(const string &name);

  DECLARE_SET_UNIFORM(1i, int x)
  DECLARE_SET_UNIFORM(2i, int x, int y)
  DECLARE_SET_UNIFORM(3i, int x, int y, int z)
  DECLARE_SET_UNIFORM(4i, int x, int y, int z, int w)

  DECLARE_SET_UNIFORM(1f, float x)
  DECLARE_SET_UNIFORM(2f, float x, float y)
  DECLARE_SET_UNIFORM(3f, float x, float y, float z)
  DECLARE_SET_UNIFORM(4f, float x, float y, float z, float w)

  void SetUniform3f(const string &name, glm::vec3 v) {
    SetUniform3f(name, v.x, v.y, v.z);
  }
  void SetUniformTexture(const string &name, const Texture &tex, int index = 0);
  void SetUniformTexture(const string &name, GLuint handle, GLuint type = GL_TEXTURE_2D ,int index = 0);
  void SetUniformMatrix4fv(const string &name, const glm::mat4 &matrix);

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

  void load(const string &path);
  void load(std::initializer_list<string> paths);
  void load(const vector<string> &paths);

  string m_name;
  GLuint m_handle;
  set<shared_ptr<Shader>> m_shaders;

  static set<shared_ptr<Program>> s_programs;
};

}  // namespace Graphics
}  // namespace WhiteDeer
