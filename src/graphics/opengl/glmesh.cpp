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

void Mesh::Draw() {
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  setVertexAttrib();
  if (m_useebo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glDrawElements(GL_TRIANGLES, m_vertexcount, GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, m_vertexcount);
  }

  for (int i = 0; i < m_attribs.size(); i++) {
    glDisableVertexAttribArray(i);
  }
}

void Mesh::load(const string& path) {
  if (Utils::endswith(path, ".ply")) {
    loadPly(path);
  } else if (Utils::endswith(path, ".obj")) {
    loadObj(path);
  } else if (path == "cube") {
    loadCube();
  } else if (path == "quad") {
    loadQuad();
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
  GLuint vertex_count, face_count;
  string temp;
  while (f >> temp) {
    if (temp == "element") {
      f >> temp;
      GLuint& count = (temp == "vertex") ? vertex_count : face_count;
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
  ebo.reserve(3 * face_count);
  unsigned v1, v2, v3, num;
  for (unsigned i = 0; i < face_count; i++) {
    f >> num >> v1 >> v2 >> v3;
    ebo.push_back(v1);
    ebo.push_back(v2);
    ebo.push_back(v3);
  }

  // prepare gl data
  prepareData(vbo, ebo, m_attribs);
}

void Mesh::loadObj(const string& path) {
  using Engine::GetLocalFileSystem;
  auto abspath = GetLocalFileSystem()->ToAbsolute(path);
  std::fstream f(abspath);

  if (!f.is_open()) {
    LOGE << "open path failed: " << path;
    return;
  }

  string temp;
  vector<float> pos, normal, tex, vbo;
  while (f >> temp) {
    float x, y, z;
    if (temp == "mtllib") {
      string mtlpath;
      f >> mtlpath;
    } else if (temp == "v") {
      f >> x >> y >> z;
      pos.push_back(x);
      pos.push_back(y);
      pos.push_back(z);
    } else if (temp == "vn") {
      f >> x >> y >> z;
      normal.push_back(x);
      normal.push_back(y);
      normal.push_back(z);
    } else if (temp == "vt") {
      f >> x >> y;
      tex.push_back(x);
      tex.push_back(y);
    } else if (temp == "f") {
      char c;
      int v1, n1, t1, v2, n2, t2, v3, n3, t3;
      f >> v1 >> c >> t1 >> c >> n1;
      f >> v2 >> c >> t2 >> c >> n2;
      f >> v3 >> c >> t3 >> c >> n3;
      vbo.push_back(pos[3 * v1 - 3]);
      vbo.push_back(pos[3 * v1 - 2]);
      vbo.push_back(pos[3 * v1 - 1]);
      vbo.push_back(normal[3 * n1 - 3]);
      vbo.push_back(normal[3 * n1 - 2]);
      vbo.push_back(normal[3 * n1 - 1]);
      vbo.push_back(tex[2 * t1 - 2]);
      vbo.push_back(tex[2 * t1 - 1]);

      vbo.push_back(pos[3 * v2 - 3]);
      vbo.push_back(pos[3 * v2 - 2]);
      vbo.push_back(pos[3 * v2 - 1]);
      vbo.push_back(normal[3 * n2 - 3]);
      vbo.push_back(normal[3 * n2 - 2]);
      vbo.push_back(normal[3 * n2 - 1]);
      vbo.push_back(tex[2 * t2 - 2]);
      vbo.push_back(tex[2 * t2 - 1]);

      vbo.push_back(pos[3 * v3 - 3]);
      vbo.push_back(pos[3 * v3 - 2]);
      vbo.push_back(pos[3 * v3 - 1]);
      vbo.push_back(normal[3 * n3 - 3]);
      vbo.push_back(normal[3 * n3 - 2]);
      vbo.push_back(normal[3 * n3 - 1]);
      vbo.push_back(tex[2 * t3 - 2]);
      vbo.push_back(tex[2 * t3 - 1]);
    }
  }

  // attribs
  m_attribs.clear();
  m_attribs.emplace_back(3, GL_FLOAT);
  m_attribs.emplace_back(3, GL_FLOAT);
  m_attribs.emplace_back(2, GL_FLOAT);

  prepareData(vbo, {}, m_attribs);
}

void Mesh::loadCube() {
  vector<GLfloat> vbo{
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
  VertexAttribList attribs = {
      {3, GL_FLOAT},
      {2, GL_FLOAT},
  };
  prepareData(vbo, {}, attribs);
}

void Mesh::loadQuad() {
  // 3        2
  // +--------+
  // |        |
  // |        |
  // +--------+
  // 0        1
  vector<GLfloat> vbo{
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
      1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
  };

  vector<GLuint> ebo{0, 2, 3, 2, 0, 1};
  VertexAttribList attribs = {{3, GL_FLOAT}, {2, GL_FLOAT}};
  prepareData(vbo, ebo, attribs);
}

void Mesh::prepareData(const vector<GLfloat>& vbo, const vector<GLuint>& ebo,
                       const VertexAttribList& attribs) {
  m_attribs = attribs;

  // submit vbo buffer
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(vbo[0]), &vbo[0],
               GL_STATIC_DRAW);
  // todo: now only triangle
  m_vertexcount = (int)vbo.size() / GetVertexSize();
  m_useebo = false;

  // submit ebo buffer
  if (!ebo.empty()) {
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo.size() * sizeof(ebo[0]), &ebo[0],
                 GL_STATIC_DRAW);
    m_vertexcount = (int)ebo.size();
    m_useebo = true;
  }
}

int Mesh::GetVertexSize() {
  // cal stride by sum up all attribs size,
  int stride = 0;
  for (int i = 0; i < m_attribs.size(); i++) {
    stride += m_attribs[i].count;
  }
  return stride;
}

int Mesh::GetVertexStride() {
  // cal stride by sum up all attribs size,
  int stride = 0;
  for (int i = 0; i < m_attribs.size(); i++) {
    stride += m_attribs[i].GetSize();
  }
  return stride;
}

void Mesh::setVertexAttrib() {
  // set all attribs
  int offset = 0;
  int stride = GetVertexStride();
  for (int i = 0; i < m_attribs.size(); i++) {
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, m_attribs[i].count, m_attribs[i].type, GL_FALSE,
                          stride, (void*)(size_t)offset);
    offset += m_attribs[i].GetSize();
  }
}

}  // namespace Graphics
}  // namespace WhiteDeer