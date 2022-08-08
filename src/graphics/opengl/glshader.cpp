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

map<string, shared_ptr<Shader>> Shader::m_shaders;

Shader::Shader(const string& path): m_name(path) {
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
  if (reload || m_shaders.find(path) == m_shaders.end()) {
    m_shaders[path] = std::make_shared<Shader>(path);
  }
  return m_shaders[path];
}

void Shader::Delete(const string& path) {
  if (m_shaders.find(path) == m_shaders.end()) {
    m_shaders.erase(path);
  }
}

void Shader::ReloadAll() {
  for (auto pairs : m_shaders) {
    Shader::Load(pairs.first, true);
  }
}

set<shared_ptr<Program>> Program::m_programs;

Program::Program(const string& pathvs, const string& pathfs) {
  m_handle = glCreateProgram();
  _Load(pathvs, pathfs);
}

void Program::_Load(const string& pathvs, const string& pathfs) {
  // todo: more shaders
  m_shaders.insert(Shader::Load(pathvs));
  m_shaders.insert(Shader::Load(pathfs));

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
  _Load(vec[0], vec[1]);
}

shared_ptr<Program> Program::Load(const string& pathvs, const string& pathfs) {
  auto p_program = std::make_shared<Program>(pathvs, pathfs);
  m_programs.insert(p_program);
  return p_program;
}

void Program::RefreshAll() {
  Shader::ReloadAll();
  for (auto& p : m_programs) {
    p->Refresh();
  }
}

}  // namespace Graphics
}  // namespace WhiteDeer