#include "graphics/opengl/glshader.h"

#include <fstream>
#include <sstream>

#include "filesystem/localfilesystem.h"
#include "graphics/opengl/glerror.h"
#include "log/log.h"
#include "utils/common/string.h"

namespace WhiteDeer {

namespace Utils {
using namespace WhiteDeer::Graphics;
map<string, shared_ptr<Shader>> Shader::s_entries;
}  // namespace Utils

namespace Graphics {

using namespace WhiteDeer::Utils;
using namespace WhiteDeer::Engine;

void Shader::load(const string &path) {
  m_name = path;
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
  load(buffer.str(), shaderType);
}

void Shader::load(const string &data, int type) {
  auto cstr = data.c_str();
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

void Shader::ReloadAll() {
  vector<string> temp;
  for (auto pairs : s_entries) {
    temp.push_back(pairs.first);
  }
  s_entries.clear();

  for (auto path : temp) {
    Shader::GetOrLoad(path);
  }
}

set<shared_ptr<Program>> Program::s_programs;

void Program::load(std::initializer_list<string> paths) {
  vector<string> temp(paths);
  load(temp);
}

void Program::load(const vector<string> &paths) {
  for (auto &path : paths) {
    m_shaders.insert(Shader::GetOrLoad(path));
  }

  for (auto &p : m_shaders) {
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
    LOGD.printf("Uniform #%d Type: %u Name: %s Length: %d Size: %d", i, type,
                name, length, size);
  }
}

void Program::load(const string &path) {
  auto localfs = GetLocalFileSystem();
  auto abspath = localfs->ToAbsolute(path);

  if (!fs::exists(abspath)) {
    LOGE << abspath << " not exists";
  }

  m_name = path;

  std::ifstream f(abspath);
  string vs_contents, fs_contents;

  string line;
  auto is_fs = false;
  while (std::getline(f, line)) {
    if (line == "// fs") {
      is_fs = true;
      continue;
    }

    auto& content = is_fs ? fs_contents : vs_contents;
    content += line;
  }

  m_shaders.insert(std::make_shared<Shader>(vs_contents, GL_VERTEX_SHADER));
  m_shaders.insert(std::make_shared<Shader>(fs_contents, GL_FRAGMENT_SHADER));

  for (auto &p : m_shaders) {
    glAttachShader(m_handle, p->m_handle);
  }
  glLinkProgram(m_handle);
}

Program::~Program() { glDeleteProgram(m_handle); }

void Program::Refresh() {
  m_shaders.clear();

  if (m_name.empty()) {
    vector<string> vec;
    for (auto &p : m_shaders) {
      glDetachShader(m_handle, p->m_handle);
      vec.push_back(p->m_name);
    }

    // reload
    load(vec);
  } else {
    load(m_name);
  }
}

void Program::RefreshAll() {
  Shader::ReloadAll();
  for (auto &p : s_programs) {
    p->Refresh();
  }
}

bool Program::HasUniform(const string &name) {
  return glGetUniformLocation(m_handle, name.c_str()) >= 0;
}

int Program::GetUniformLocation(const string &name) {
  auto result = glGetUniformLocation(m_handle, name.c_str());
  if (result < 0) {
    // LOGW << name << ": " << result;
  }
  return result;
}

void Program::SetUniformTexture(const string &name, const Texture &tex,
                                int index) {
  if (!HasUniform(name)) return;
  tex.Use(index);
  glUniform1i(GetUniformLocation(name), index);
}

void Program::SetUniformTexture(const string &name, GLuint handle, GLuint type, int index) {
  if (!HasUniform(name)) return;
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(type, handle);
  glUniform1i(GetUniformLocation(name), index);
}

void Program::SetUniformMatrix4fv(const string &name, const glm::mat4 &matrix) {
  if (!HasUniform(name)) return;
  glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE,
                     glm::value_ptr(matrix));
}

}  // namespace Graphics
}  // namespace WhiteDeer
