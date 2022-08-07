#include "graphics/opengl/glshader.h"

#include <fstream>
#include <sstream>

#include "filesystem/localfilesystem.h"
#include "log/log.h"
#include "utils/common/string.h"

namespace WhiteDeer {
namespace Graphics {

using namespace WhiteDeer::Utils;
using namespace WhiteDeer::Engine;

map<string, shared_ptr<Shader>> Shader::m_shaders;

Shader::Shader(const string& path) {
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
}

Shader::~Shader() {
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

set<shared_ptr<Program>> Program::m_programs;

Program::Program() { m_handle = glCreateProgram(); }

Program::~Program() { glDeleteProgram(m_handle); }

shared_ptr<Program> Program::Load(const string& path) {
  auto p_program = std::make_shared<Program>();

  // todo: more shaders
  auto p_shader = Shader::Load(path);
  glAttachShader(p_program->m_handle, p_shader->m_handle);
  glLinkProgram(p_program->m_handle);

  m_programs.insert(p_program);
  return p_program;
}

}  // namespace Graphics
}  // namespace WhiteDeer