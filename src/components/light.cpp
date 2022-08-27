#include "components/light.h"

#include "components/transform.h"
#include "scene/gameobject.h"

namespace WhiteDeer {
namespace Engine {

void Light::Use(shared_ptr<Program> program) {
  auto transform = GetGameObject()->GetComponent<Transform>();
  program->SetUniform1i("u_light.type", m_type);
  program->SetUniform3f("u_light.pos", transform->GetPosition());
  program->SetUniform3f("u_light.dir", transform->GetZDirection());
  program->SetUniform3f("u_light.color", GetColor());

  if (m_type != LightType_Direction) {
    program->SetUniform1f("u_light.constant", m_constant);
    program->SetUniform1f("u_light.linear", m_linear);
    program->SetUniform1f("u_light.quadratic", m_quadratic);
    if (m_type == LightType_Spot) {
      program->SetUniform1f("u_light.cutoff", glm::cos(glm::radians(m_cutoff)));
      program->SetUniform1f("u_light.outerCutoff", glm::cos(glm::radians(m_outerCutoff)));
    }
  }
}

}  // namespace Engine
}  // namespace WhiteDeer