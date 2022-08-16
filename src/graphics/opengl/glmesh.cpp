#include "graphics/opengl/glmesh.h"

#include <fstream>

#include "filesystem/localfilesystem.h"
#include "log/log.h"
#include "utils/common/string.h"

namespace WhiteDeer {

namespace Utils {
using WhiteDeer::Graphics::Mesh;
map<string, shared_ptr<Mesh>> Mesh::s_entries;
}  // namespace Utils

namespace Graphics {

void Mesh::load(const string& path) {
  if (Utils::endswith(path, ".ply")) {
    loadPly(path);
  } else if (Utils::endswith(path, ".obj")) {
    loadObj(path);
  }
}

void Mesh::loadPly(const string& path) {
  using Engine::GetLocalFileSystem;
  auto localfs = GetLocalFileSystem();
  auto pathstring = localfs->ToAbsolute(path);

  std::fstream f(pathstring);
  if (!f.is_open()) {
    LOGE << "open path failed: " << path;
    return;
  }

  // read header
  GLuint vertex_count;
  string temp;
  while (f >> temp) {
    if (temp == "element") {
      f >> temp;
      GLuint& count = (temp == "vertex") ? vertex_count : m_facecount;
      f >> count;
    }

    if (temp == "end_header") {
      break;
    }
  }

  // vertex attrib
  // todo: read from header
  m_attribs.clear();
  m_attribs.emplace_back(3, GL_FLOAT);

  // read vertex
  vector<GLfloat> vbo;
  vbo.reserve(3 * vertex_count);
  float x, y, z, _t1, _t2;
  for (unsigned i = 0; i < vertex_count; i++) {
    f >> x >> y >> z >> _t1 >> _t2;
    vbo.push_back(x);
    vbo.push_back(y);
    vbo.push_back(z);
  }

  // read triangles
  vector<GLuint> ebo;
  ebo.reserve(3 * m_facecount);
  unsigned v1, v2, v3, num;
  for (unsigned i = 0; i < m_facecount; i++) {
    f >> num >> v1 >> v2 >> v3;
    ebo.push_back(v1);
    ebo.push_back(v2);
    ebo.push_back(v3);
  }

  // prepare gl data
  prepareData(vbo, ebo);
}

void Mesh::loadObj(const string& path) {
  using Engine::GetLocalFileSystem;
  auto abspath = GetLocalFileSystem()->ToAbsolute(path);
  std::fstream f(abspath);

  if (!f.is_open()) {
    LOGE << "open path failed: " << path;
    return;
  }


}

void Mesh::prepareData(const vector<GLfloat>& vbo, const vector<GLuint>& ebo) {
  // submit vbo buffer
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(vbo[0]), &vbo[0],
               GL_STATIC_DRAW);

  // submit ebo buffer
  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo.size() * sizeof(ebo[0]), &ebo[0],
               GL_STATIC_DRAW);
}

void Mesh::setVertexAttrib() {
  // cal stride by sum up all attribs size,
  int stride = 0;
  for (int i = 0; i < m_attribs.size(); i++) {
    stride += m_attribs[i].GetSize();
  }

  // set all attribs
  int offset = 0;
  for (int i = 0; i < m_attribs.size(); i++) {
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, m_attribs[i].count, m_attribs[i].type, GL_FALSE,
                          stride, (void*)(size_t)offset);
    offset += m_attribs[i].GetSize();
  }
}

// todo: move to render pipeline
void Mesh::Draw() {
  setVertexAttrib();
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawElements(GL_TRIANGLES, m_facecount * 3, GL_UNSIGNED_INT, 0);
}

}  // namespace Graphics

}  // namespace WhiteDeer