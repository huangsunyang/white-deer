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

void Postprocess::Render(const Texture& from, const RenderTexture& t) {
  // apply postprocess on texture t
  // render to current framebuffer
  auto lastFrameBuffer = FrameBuffer::GetCurrent();
  auto tempFrameBuffer = FrameBuffer::GetOrLoad("temp");
  tempFrameBuffer->BindColor(t);
  m_program->Use();
  m_program->SetUniformTexture("u_screen", from);
  m_quad->Draw();
  lastFrameBuffer->RawBind();
}

}  // namespace Graphics
}  // namespace WhiteDeer