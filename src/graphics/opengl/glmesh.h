#pragma once

#include <GL/gl3w.h>
#include <vector>
using std::vector;

namespace WhiteDeer {
namespace Graphics {

class Mesh {
 public:

 protected:
   void Load();
   void LoadPly();

   vector<float> m_vbo;
   vector<float> m_ebo;
};

}  // namespace Graphics
}  // namespace WhiteDeer
