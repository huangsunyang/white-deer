#include "graphics/opengl/glshader.h"

#include <fstream>
#include <sstream>

#include "filesystem/localfilesystem.h"
#include "graphics/opengl/glerror.h"
#include "log/log.h"
#include "utils/common/string.h"

namespace WhiteDeer {
namespace Graphics {

using namespace WhiteDeer::Utils;
using namespace WhiteDeer::Engine;

map<string, shared_ptr<Shader>> Shader::s_shaders;

Shader::Shader(const string& path) : m_name(path) {
  auto localfs = GetLocalFileSystem();
  auto abspath = localfs->ToAbsolute(path);

  if (!fs::exists(abspath)) {
    LOGE << abspath << " not exists";
  }

  // todo: more shader type
  auto shaderType =
      endswith(path, ".vs") ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
  m_handle = glCreateShader(shaderType);

  std::fstream f(abspath.string());
  std::stringstream buffer;
  buffer << f.rdbuf();
  auto str = buffer.str();
  auto cstr = str.c_str();
  glShaderSource(m_handle, 1, &cstr, NULL);
  glCompileShader(m_handle);

  // check shader compile
  int success;
  char infoLog[512];
  glGetShaderiv(m_handle, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(m_handle, 512, NULL, infoLog);
    LOGE << "SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
}

Shader::~Shader() {
  LOGD << "deleteing shader " << m_name;
  if (m_handle) glDeleteShader(m_handle);
}

shared_ptr<Shader> Shader::Load(const string& path, bool reload) {
  if (reload || s_shaders.find(path) == s_shaders.end()) {
    s_shaders[path] = std::make_shared<Shader>(path);
  }
  return s_shaders[path];
}

void Shader::Delete(const string& path) {
  if (s_shaders.find(path) == s_shaders.end()) {
    s_shaders.erase(path);
  }
}

void Shader::ReloadAll() {
  for (auto pairs : s_shaders) {
    Shader::Load(pairs.first, true);
  }
}

set<shared_ptr<Program>> Program::s_programs;

void Program::_Load(std::initializer_list<string> paths) {
  vector<string> temp(paths);
  _Load(temp);
}

void Program::_Load(const vector<string>& paths) {
  for (auto& path : paths) {
    m_shaders.insert(Shader::Load(path));
  }

  for (auto& p : m_shaders) {
    glAttachShader(m_handle, p->m_handle);
  }
  glLinkProgram(m_handle);

  // check link status
  int success;
  char infoLog[512];
  glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(m_handle, 512, NULL, infoLog);
    LOGE << "PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
  }

  // query all uniforms
  int count;
  glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count);
  for (int i = 0; i < count; i++) {
    char name[32];
    GLint length;
    GLsizei size;
    GLenum type;
    glGetActiveUniform(m_handle, (GLuint)i, 32, &length, &size, &type, name);
    LOGD.printf("Uniform #%d Type: %u Name: %s Length: %d Size: %d\n", i, type,
                name, length, size);
  }
}

Program::~Program() { glDeleteProgram(m_handle); }

void Program::Refresh() {
  vector<string> vec;
  for (auto& p : m_shaders) {
    glDetachShader(m_handle, p->m_handle);
    vec.push_back(p->m_name);
  }
  m_shaders.clear();

  // reload
  _Load(vec);
}

void Program::RefreshAll() {
  Shader::ReloadAll();
  for (auto& p : s_programs) {
    p->Refresh();
  }
}

}  // namespace Graphics
}  // namespace WhiteDeer

using namespace WhiteDeer::Graphics;
RTTR_REGISTRATION {
  rttr::registration::class_<Program>("WhiteDeer::Graphics::Program")
      .property("shaders", &Program::m_shaders,
                rttr::registration::private_access)
      .property("handle", &Program::m_handle,
                rttr::registration::private_access);

  rttr::registration::class_<Shader>("WhiteDeer::Graphics::Shader")
      .property("name", &Shader::m_name, rttr::registration::private_access)
      .property("handle", &Shader::m_handle,
                rttr::registration::private_access);
}