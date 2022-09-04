#include "graphics/postprocess.h"

#include <fmt/format.h>

namespace WhiteDeer {

namespace Utils {

using WhiteDeer::Graphics::Postprocess;
map<PostprocessType, shared_ptr<Postprocess>> Postprocess::s_entries;

}  // namespace Utils

namespace Graphics {

Postprocess::Postprocess(PostprocessType t) {
  m_type = t;
  m_quad = Mesh::GetOrLoad("quad");
  // PostprocessType_{name}
  string name = EnumToString(t).substr(16);
  for (auto& c : name) {
    c = std::tolower(c);
  }
  m_program =
      Program::Load("package/shaders/postprocess/postprocess.vs",
                    fmt::format("package/shaders/postprocess/{}.fs", name));
}

void Postprocess::Render(const RenderTexture& to,
                         std::initializer_list<const Texture *> froms) {
  // apply postprocess on texture t
  // render to current framebuffer
  auto lastFrameBuffer = FrameBuffer::GetCurrent();
  auto tempFrameBuffer = FrameBuffer::GetOrLoad("temp");
  tempFrameBuffer->BindColor(to);
  m_program->Use();

  // set texture uniforms
  int i = 0;
  for (auto& from : froms) {
    if (i == 0) {
      m_program->SetUniformTexture("u_screen", *from, i);
    }
    m_program->SetUniformTexture(fmt::format("u_screen{0}", i), *from, i);
    i++;
  }
  m_quad->Draw();
  lastFrameBuffer->RawBind();
}

}  // namespace Graphics
}  // namespace WhiteDeer